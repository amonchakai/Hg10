/*
 * FileTransfert.hpp
 *
 *  Created on: 20 déc. 2014
 *      Author: pierre
 */

#ifndef FILETRANSFERT_HPP_
#define FILETRANSFERT_HPP_

#include <QString>

class FileTransfert {

public:
    virtual ~FileTransfert() {}

public:
    virtual void putFile       (const QString &path) = 0;
    virtual void share         () = 0;


};



#endif /* FILETRANSFERT_HPP_ */
