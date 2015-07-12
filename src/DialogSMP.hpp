/*
 * DialogSMP.hpp
 *
 *  Created on: 12 juil. 2015
 *      Author: pierre
 */

#ifndef DIALOGSMP_HPP_
#define DIALOGSMP_HPP_


#include <bb/cascades/CustomControl>

using namespace bb::cascades;

namespace bb
{
    namespace cascades
    {
        class Dialog;
        class TextField;
    }
}



class DialogSMP : public QObject
{
    Q_OBJECT

public:
    DialogSMP(QObject * parent = 0);
    virtual ~DialogSMP();

    void setVisible(bool visibility);


Q_SIGNALS:
    void validated(const QString& question, const QString& secret);


private slots:


    void hideDialog();
    void validDialog();

private:
    // Whether the custom dialog is visible or not
    bool mVisible;

    // The custom dialog
    Dialog      *mCustomDialog;
    TextField   *m_QuestionTextField;
    TextField   *m_SecretTextField;


};


#endif /* DIALOGSMP_HPP_ */
