/*
 * SmileyPickerController.cpp
 *
 *  Created on: 5 avr. 2014
 *      Author: pierre
 */
#include "SmileyPickerController.hpp"



#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QRegExp>

#include <bb/cascades/Application>
#include <bb/cascades/ThemeSupport>
#include <bb/cascades/ColorTheme>
#include <bb/cascades/Theme>


static const QString defaultSmiley = QString("<table>")
										+ "<tr>"
											+"<td><img src=\"smile.png\" width=\"40%\" height=\"40%\" alt=\":)\" title=\":)\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>"
											+"<td><img src=\"blink.png\" width=\"40%\" height=\"40%\" alt=\";)\" title=\";)\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>"
											+"<td><img src=\"bigsmile.png\" width=\"40%\" height=\"40%\" alt=\":D\" title=\":D\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>"
											+"<td><img src=\"lol.png\" width=\"40%\" height=\"40%\" alt=\"=)\" title=\"=)\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>"
											+"<td><img src=\"lol2.png\" width=\"40%\" height=\"40%\" alt=\"=))\" title=\"=))\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>"
											+"<td><img src=\"bighug.png\" width=\"40%\" height=\"40%\" alt=\"({})\" title=\":ange:\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>"
										+ "</tr>"
										+ "<tr>"
											+"<td><img src=\"dancing.png\" width=\"40%\" height=\"40%\" alt=\"\\=D/\" title=\"\\=D/\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>"
											+"<td><img src=\"tongue.png\" width=\"40%\" height=\"40%\" alt=\":P\" title=\":P\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>"
											+"<td><img src=\"eyelash.png\" width=\"40%\" height=\"40%\" alt=\";;)\" title=\";;)\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>"
											+"<td><img src=\"love.png\" width=\"40%\" height=\"40%\" alt=\"<3<3\" title=\"<3<3\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>"
											+"<td><img src=\"kiss.png\" width=\"40%\" height=\"40%\" alt=\":*\" title=\":*\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>"
											+"<td><img src=\"sarcastic.png\" width=\"40%\" height=\"40%\" alt=\":>\" title=\":>\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>"
										+ "</tr>"
										+ "<tr>"
											+"<td><img src=\"noInterest.png\" width=\"40%\" height=\"40%\" alt=\"3-|\" title=\"3-|\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>"
											+"<td><img src=\"notTalking.png\" width=\"40%\" height=\"40%\" alt=\":]x\" title=\":]x\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>"
											+"<td><img src=\"stop.png\" width=\"40%\" height=\"40%\" alt=\">:/\" title=\">:/\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>"
											+"<td><img src=\"yawn.png\" width=\"40%\" height=\"40%\" alt=\"(=|\" title=\"(=|\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>"
											+"<td><img src=\"seald.png\" width=\"40%\" height=\"40%\" alt=\":x\" title=\":x\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>"
											+"<td><img src=\"question.png\" width=\"40%\" height=\"40%\" alt=\":/\" title=\":/\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>"
										+ "</tr>"
										+ "<tr>"
											+"<td><img src=\"oh.png\" width=\"40%\" height=\"40%\" alt=\":o\" title=\":o\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>"
											+"<td><img src=\"facepalm.png\" width=\"40%\" height=\"40%\" alt=\"X_X\" title=\"X_X\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>"
											+"<td><img src=\"poker.png\" width=\"40%\" height=\"40%\" alt=\":|\" title=\":|\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>"
											+"<td><img src=\"eyebrow.png\" width=\"40%\" height=\"40%\" alt=\"/:)\" title=\"/:)\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>"
											+"<td><img src=\"angel.png\" width=\"40%\" height=\"40%\" alt=\"O:)\" title=\"O:)\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>"
											+"<td><img src=\"sol.png\" width=\"40%\" height=\"40%\" alt=\"B-)\" title=\"B-)\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>"
										+ "</tr>"
										+ "<tr>"
											+"<td><img src=\"sad.png\" width=\"40%\" height=\"40%\" alt=\":(\" title=\":(\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>"
											+"<td><img src=\"demon.png\" width=\"40%\" height=\"40%\" alt=\">=)\" title=\">=)\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>"
											+"<td><img src=\"cry.png\" width=\"40%\" height=\"40%\" alt=\":'(\" title=\":'(\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>"
											+"<td><img src=\"sleep.png\" width=\"40%\" height=\"40%\" alt=\"Z_Z\" title=\"Z_Z\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>"
											+"<td><img src=\"confused.png\" width=\"40%\" height=\"40%\" alt=\":s\" title=\":s\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>"
											+"<td><img src=\"dunno.png\" width=\"40%\" height=\"40%\" alt=\":]Y\" title=\":]Y\" onclick=\"navigator.cascades.postMessage(this.alt)\" /></td>"
										+ "</tr>"
								  + "</table>";

void SmileyPickerController::loadDefautSmiley() {
	updateView(defaultSmiley);
}


void SmileyPickerController::updateView(const QString &webpage) {
	if(m_WebView == NULL) {
		qWarning() << "error webview was not provided or not a webview";
		return;
	}


	QString colorHandling = "} ";
	if(bb::cascades::Application::instance()->themeSupport()->theme()->colorTheme()->style() == bb::cascades::VisualStyle::Dark) {
		colorHandling = "background-color:#000000; color:#FFFFFF; } ";
	}

	m_WebView->setHtml(QString("<!DOCTYPE html><html><head><style type=\"text/css\">")
				+ "table {table-layout:fixed; width: 800px; border-spacing: 30px; } th {text-align:left; text-decoration:underline;} td {width: 30px; overflow: hidden; }"	// render quotation table
	            + "body {font-size:25px; " + colorHandling  // switch webview color based on theme
	            + "p {font-size:25px;} "
			+ "</style>"
			+ "</head><body>"
			+ "<center>" + webpage + "</center></body></html>", QUrl("local:///assets/images/smiley/"));
}


