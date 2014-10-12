/****************************************************************************
**
** Copyright (C) 2012 Jeremy Lainé <jeremy.laine@m4x.org>
** Contact: http://www.qt-project.org/
**
** This file is part of the QtNetwork module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qdnslookup_p.h"

#include <QLibrary>
#include <QMutex>
#include <QScopedPointer>
#include <QUrl>

#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/nameser.h>
#include <arpa/nameser_compat.h>
#include <resolv.h>


QT_BEGIN_NAMESPACE

static QMutex local_res_mutex;

// Custom deleter to close resolver state.

struct QDnsLookupStateDeleter
{
    static inline void cleanup(struct __res_state *pointer)
    {
        res_nclose(pointer);
    }
};

static void resolveLibrary()
{
    // nothing to do... Just use system API!
}

void QDnsLookupRunnable::query(const int requestType, const QByteArray &requestName, QDnsLookupReply *reply)
{
    // Load dn_expand, res_ninit and res_nquery on demand.
    static volatile bool triedResolve = false;
    if (!triedResolve) {
        QMutexLocker locker(&local_res_mutex);
        if (!triedResolve) {
            resolveLibrary();
            triedResolve = true;
        }
    }

    // Initialize state.
   // struct __res_state state;
    //memset(&state, 0, sizeof(state));
    /*
    if (res_ninit(&state) < 0) {
        reply->error = QDnsLookup::ResolverError;
        reply->errorString = tr("Resolver initialization failed");
        return;
    }

#ifdef QDNSLOOKUP_DEBUG
    state.options |= RES_DEBUG;
#endif
*/
    //QScopedPointer<struct __res_state, QDnsLookupStateDeleter> state_ptr(&state);

    // Perform DNS query.
    unsigned char response[PACKETSZ];
    memset(response, 0, sizeof(response));
    const int responseLength = res_query(requestName, C_IN, requestType, response, sizeof(response));

    // Check the response header.
    HEADER *header = (HEADER*)response;
    const int answerCount = ntohs(header->ancount);
    switch (header->rcode) {
    case NOERROR:
        break;
    case FORMERR:
        reply->error = QDnsLookup::InvalidRequestError;
        reply->errorString = tr("Server could not process query");
        return;
    case SERVFAIL:
        reply->error = QDnsLookup::ServerFailureError;
        reply->errorString = tr("Server failure");
        return;
    case NXDOMAIN:
        reply->error = QDnsLookup::NotFoundError;
        reply->errorString = tr("Non existent domain");
        return;
    case REFUSED:
        reply->error = QDnsLookup::ServerRefusedError;
        reply->errorString = tr("Server refused to answer");
        return;
    default:
        reply->error = QDnsLookup::InvalidReplyError;
        reply->errorString = tr("Invalid reply received");
        return;
    }

    // Check the reply is valid.
    if (responseLength < int(sizeof(HEADER))) {
        reply->error = QDnsLookup::InvalidReplyError;
        reply->errorString = tr("Invalid reply received");
        return;
    }

    // Skip the query host, type (2 bytes) and class (2 bytes).
    char host[PACKETSZ], answer[PACKETSZ];
    unsigned char *p = response + sizeof(HEADER);
    int status = dn_expand(response, response + responseLength, p, host, sizeof(host));
    if (status < 0) {
        reply->error = QDnsLookup::InvalidReplyError;
        reply->errorString = tr("Could not expand domain name");
        return;
    }
    p += status + 4;

    // Extract results.
    int answerIndex = 0;
    while ((p < response + responseLength) && (answerIndex < answerCount)) {
        status = dn_expand(response, response + responseLength, p, host, sizeof(host));
        if (status < 0) {
            reply->error = QDnsLookup::InvalidReplyError;
            reply->errorString = tr("Could not expand domain name");
            return;
        }
        const QString name = QUrl::fromAce(host);

        p += status;
        const quint16 type = (p[0] << 8) | p[1];
        p += 2; // RR type
        p += 2; // RR class
        const quint32 ttl = (p[0] << 24) | (p[1] << 16) | (p[2] << 8) | p[3];
        p += 4;
        const quint16 size = (p[0] << 8) | p[1];
        p += 2;

        if (type == QDnsLookup::A) {
            if (size != 4) {
                reply->error = QDnsLookup::InvalidReplyError;
                reply->errorString = tr("Invalid IPv4 address record");
                return;
            }
            const quint32 addr = (p[0] << 24) | (p[1] << 16) | (p[2] << 8) | p[3];
            QDnsHostAddressRecord record;
            record.d->name = name;
            record.d->timeToLive = ttl;
            record.d->value = QHostAddress(addr);
            reply->hostAddressRecords.append(record);
        } else if (type == QDnsLookup::AAAA) {
            if (size != 16) {
                reply->error = QDnsLookup::InvalidReplyError;
                reply->errorString = tr("Invalid IPv6 address record");
                return;
            }
            QDnsHostAddressRecord record;
            record.d->name = name;
            record.d->timeToLive = ttl;
            record.d->value = QHostAddress(p);
            reply->hostAddressRecords.append(record);
        } else if (type == QDnsLookup::CNAME) {
            status = dn_expand(response, response + responseLength, p, answer, sizeof(answer));
            if (status < 0) {
                reply->error = QDnsLookup::InvalidReplyError;
                reply->errorString = tr("Invalid canonical name record");
                return;
            }
            QDnsDomainNameRecord record;
            record.d->name = name;
            record.d->timeToLive = ttl;
            record.d->value = QUrl::fromAce(answer);
            reply->canonicalNameRecords.append(record);
        } else if (type == QDnsLookup::NS) {
            status = dn_expand(response, response + responseLength, p, answer, sizeof(answer));
            if (status < 0) {
                reply->error = QDnsLookup::InvalidReplyError;
                reply->errorString = tr("Invalid name server record");
                return;
            }
            QDnsDomainNameRecord record;
            record.d->name = name;
            record.d->timeToLive = ttl;
            record.d->value = QUrl::fromAce(answer);
            reply->nameServerRecords.append(record);
        } else if (type == QDnsLookup::PTR) {
            status = dn_expand(response, response + responseLength, p, answer, sizeof(answer));
            if (status < 0) {
                reply->error = QDnsLookup::InvalidReplyError;
                reply->errorString = tr("Invalid pointer record");
                return;
            }
            QDnsDomainNameRecord record;
            record.d->name = name;
            record.d->timeToLive = ttl;
            record.d->value = QUrl::fromAce(answer);
            reply->pointerRecords.append(record);
        } else if (type == QDnsLookup::MX) {
            const quint16 preference = (p[0] << 8) | p[1];
            status = dn_expand(response, response + responseLength, p + 2, answer, sizeof(answer));
            if (status < 0) {
                reply->error = QDnsLookup::InvalidReplyError;
                reply->errorString = tr("Invalid mail exchange record");
                return;
            }
            QDnsMailExchangeRecord record;
            record.d->exchange = QUrl::fromAce(answer);
            record.d->name = name;
            record.d->preference = preference;
            record.d->timeToLive = ttl;
            reply->mailExchangeRecords.append(record);
        } else if (type == QDnsLookup::SRV) {
            const quint16 priority = (p[0] << 8) | p[1];
            const quint16 weight = (p[2] << 8) | p[3];
            const quint16 port = (p[4] << 8) | p[5];
            status = dn_expand(response, response + responseLength, p + 6, answer, sizeof(answer));
            if (status < 0) {
                reply->error = QDnsLookup::InvalidReplyError;
                reply->errorString = tr("Invalid service record");
                return;
            }
            QDnsServiceRecord record;
            record.d->name = name;
            record.d->target = QUrl::fromAce(answer);
            record.d->port = port;
            record.d->priority = priority;
            record.d->timeToLive = ttl;
            record.d->weight = weight;
            reply->serviceRecords.append(record);
        } else if (type == QDnsLookup::TXT) {
            unsigned char *txt = p;
            QDnsTextRecord record;
            record.d->name = name;
            record.d->timeToLive = ttl;
            while (txt < p + size) {
                const unsigned char length = *txt;
                txt++;
                if (txt + length > p + size) {
                    reply->error = QDnsLookup::InvalidReplyError;
                    reply->errorString = tr("Invalid text record");
                    return;
                }
                record.d->values << QByteArray((char*)txt, length);
                txt += length;
            }
            reply->textRecords.append(record);
        }
        p += size;
        answerIndex++;
    }
}

QT_END_NAMESPACE
