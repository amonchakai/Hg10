/*
 * DialogSMP.cpp
 *
 *  Created on: 12 juil. 2015
 *      Author: pierre
 */


#include "DialogSMP.hpp"

#include <bb/cascades/Color>
#include <bb/cascades/Container>
#include <bb/cascades/Dialog>
#include <bb/cascades/DockLayout>
#include <bb/cascades/ImageView>
#include <bb/cascades/Label>
#include <bb/cascades/StackLayout>
#include <bb/cascades/StackLayoutProperties>
#include <bb/cascades/SystemDefaults>
#include <bb/cascades/TextStyle>
#include <bb/cascades/TextField>
#include <bb/cascades/Button>

#include <bb/cascades/Application>
#include <bb/cascades/ThemeSupport>
#include <bb/cascades/ColorTheme>
#include <bb/cascades/Theme>



DialogSMP::DialogSMP(QObject * parent) : QObject(parent) {
    bool connectResult;
    Q_UNUSED(connectResult);

    using namespace bb::cascades;

    mCustomDialog = new Dialog();

    // Get the UIConfig object in order to use resolution independent sizes.
    UIConfig *ui = mCustomDialog->ui();

    Container *contentContainer = new Container();

    contentContainer->setLayout(new DockLayout());
    contentContainer->setHorizontalAlignment(HorizontalAlignment::Fill);
    contentContainer->setVerticalAlignment(VerticalAlignment::Fill);

    // The background is set to semi-transparent to indicate that it is not possible to interact
    // with the screen behind the dialog.
    contentContainer->setBackground(Color::fromARGB(0x88000000));

    // Dialog Container
    Container *dialogContainer = new Container();
    dialogContainer->setLayout(new DockLayout());
    dialogContainer->setHorizontalAlignment(HorizontalAlignment::Center);
    dialogContainer->setVerticalAlignment(VerticalAlignment::Center);
    dialogContainer->setMaxHeight(ui->du(59.7));
    dialogContainer->setLeftPadding(ui->du(3));
    dialogContainer->setRightPadding(ui->du(3));

    if(bb::cascades::Application::instance()->themeSupport()->theme()->colorTheme()->style() == bb::cascades::VisualStyle::Dark) {
        dialogContainer->setBackground(Color::fromARGB(0xff282828));
    } else {
        dialogContainer->setBackground(Color::fromARGB(0xffffffff));
    }

    // Dialog content Container
    Container *dialogContent = Container::create()
                                    .top(ui->du(0.5)).bottom(ui->du(2)).left(ui->du(2));
    dialogContent->setHorizontalAlignment(HorizontalAlignment::Fill);
    dialogContent->setVerticalAlignment(VerticalAlignment::Fill);

    // Title label for dialog
    Label *dialogTitle = new Label();
    dialogTitle->setText(tr("Check the identity of your contact"));
    dialogTitle->textStyle()->setBase(SystemDefaults::TextStyles::titleText());
    //dialogTitle->textStyle()->setColor(Color::fromARGB(0xfffafafa));
    dialogTitle->setLayoutProperties(StackLayoutProperties::create().spaceQuota(1));
    dialogTitle->setHorizontalAlignment(HorizontalAlignment::Center);

    // Text label for dialog
    Label *dialogText = new Label();
    dialogText->setText(tr("Please specify the question you want him to reply. This field can be empty."));
    dialogText->setMultiline(true);

    // Toggle button for alarm
    m_QuestionTextField = new TextField();
    m_QuestionTextField->setLayoutProperties(StackLayoutProperties::create().spaceQuota(1));
    m_QuestionTextField->setHorizontalAlignment(HorizontalAlignment::Fill);

    // Text label for dialog
    Label *dialogText2 = new Label();
    dialogText2->setText(tr("What is the secret?"));

    // Toggle button for alarm
    m_SecretTextField = new TextField();
    m_SecretTextField->setLayoutProperties(StackLayoutProperties::create().spaceQuota(1));
    m_SecretTextField->setHorizontalAlignment(HorizontalAlignment::Fill);



    Container *buttonContainer = Container::create();
    buttonContainer->setHorizontalAlignment(HorizontalAlignment::Fill);
    StackLayout *layoutButtons = new StackLayout();
    layoutButtons->setOrientation(LayoutOrientation::LeftToRight);
    buttonContainer->setLayout(layoutButtons);
    buttonContainer->setHorizontalAlignment(HorizontalAlignment::Center);

    Button *acceptButton = new Button();
    acceptButton->setText(tr("Send request"));

    Button *cancelButton = new Button();
    cancelButton->setText(tr("Cancel"));

    buttonContainer->add(cancelButton);
    buttonContainer->add(acceptButton);

    // Add components to the appropriate Containers
    dialogContent->add(dialogTitle);
    dialogContent->add(dialogText);
    dialogContent->add(m_QuestionTextField);
    dialogContent->add(dialogText2);
    dialogContent->add(m_SecretTextField);
    dialogContent->add(buttonContainer);

    dialogContainer->add(dialogContent);
    contentContainer->add(dialogContainer);

    mCustomDialog->setContent(contentContainer);


    connectResult = QObject::connect(cancelButton, SIGNAL(clicked()), this, SLOT(hideDialog()));
    Q_ASSERT(connectResult);

    connectResult = QObject::connect(acceptButton, SIGNAL(clicked()), this, SLOT(validDialog()));
    Q_ASSERT(connectResult);


}

DialogSMP::~DialogSMP()
{
    // If the Dialog is showing make sure it is closed and reset its content
    if (mCustomDialog->isOpened()) {
        mCustomDialog->resetContent();
        mCustomDialog->close();
    }
    delete mCustomDialog;
}


void DialogSMP::setVisible(bool visible)
{
    if (mVisible != visible) {
        if (visible) {
            mCustomDialog->open();
        } else {
            mCustomDialog->close();
        }

        mVisible = visible;
    }
}

void DialogSMP::hideDialog() {
    setVisible(false);
}


void DialogSMP::validDialog() {
    hideDialog();

    emit validated(m_QuestionTextField->text(), m_SecretTextField->text());

}



