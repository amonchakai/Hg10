/*
 * SmileyPickerController.hpp
 *
 *  Created on: 5 avr. 2014
 *      Author: pierre
 */

#ifndef SMILEYPICKERCONTROLLER_HPP_
#define SMILEYPICKERCONTROLLER_HPP_

#include <QtCore/QObject>
#include <bb/cascades/ListView>
#include <QReadWriteLock>

class Sticker;

class SmileyPickerController : public QObject {
	Q_OBJECT;

	private:

	bb::cascades::ListView 				*m_ListView;
	QReadWriteLock                       m_Mutex;
	QList<Sticker *>                     m_Stickers;

	// ----------------------------------------------------------------------------------------------
	public:
		SmileyPickerController(QObject *parent = 0);
		virtual ~SmileyPickerController() {};


	// ----------------------------------------------------------------------------------------------

	public Q_SLOTS:
		void loadDefautSmiley   ();
		void getSmiley          (const QString &url_str);
		void refresh            ();

		void checkReply();
		void onImageReady       (const QString &url, const QString &diskPath);


		inline void setListView (QObject *listView) 				{m_ListView = dynamic_cast<bb::cascades::ListView*>(listView); }



	// ----------------------------------------------------------------------------------------------
	Q_SIGNALS:
		void complete       ();




	// ----------------------------------------------------------------------------------------------
	private:

		void updateView();
		void pushToView();

};




#endif /* SMILEYPICKERCONTROLLER_HPP_ */
