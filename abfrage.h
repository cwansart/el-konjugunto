/****************************************************************************
**
**	Copyright (C) 2009 Viktor Baum, Timo Kikillus, Alex Meier,
**										 Florian Porrmann, Marcel Pokraka,
**										 Sebastian Steinhorst, Niklas Stumpe,
**										 Christian Wansart, Marcel Wiegand
**
**	This file is part of El Konjungúnto.
**
**	El Konjungúnto is free software: you can redistribute it and/or modify
**	it under the terms of the GNU General Public License as published by
**	the Free Software Foundation, either version 3 of the License, or
**	(at your option) any later version.
**
**	El Konjungúnto is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**	GNU General Public License for more details.
**
**	You should have received a copy of the GNU General Public License
**	along with El Konjungúnto.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#ifndef ABFRAGE_H
#define ABFRAGE_H

// ----------------------
#include <QWidget>
#include <QApplication>
#include <QDesktopWidget>
#include <QDialog>
// ----------------------
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QRadioButton>
#include <QMessageBox>
#include <QList>
#include <QGroupBox>
#include <QTimer>
// ----------------------
#include <QGridLayout>

#include "statistik.h"
#include "userDatabase.h"


class abfrage : public QWidget
{
	Q_OBJECT

	public:
		abfrage(QStringList, const QString&, const QString&, QWidget *parent = 0);
		void newvocs();
		void center(QWidget *widget, int w, int h);

	public slots:
		void s_test();
//		void s_nje_k();
//		void s_nje_g();

	private:
		QLabel *l_titel;
		QLabel *l_subtitel;
		QLabel *l_tense;
		QLabel *l_currentTense;
		QLabel *l_vokabel;
		QLabel *l_verb;
		QPushButton *pb_input;
//		QPushButton *pb_nje_k;
//		QPushButton *pb_nje_g;
		QList<QLineEdit*> lle_input;
		QList<QLabel*> ll_result;
		QList<QLabel*> ll_status;
		QStringList sl_voc;
		QStringList sl_vocs;
		bool voc_right[6];
		bool clear;
		int counter;
		int right;
		int falsch;
		QString user;
		QString time;
};

#endif
