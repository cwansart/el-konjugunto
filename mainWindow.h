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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
#include <QGroupBox>
#include <QComboBox>
#include <QList>
#include <QStringList>
#include <QCheckBox>
#include <QScrollArea>
#include <QPixmap>
// ----------------------
#include <QGridLayout>
// ----------------------

#include "abfrage.h"
#include "statistik.h"
#include "optionen.h"
#include "vocDatabase.h"
#include "userDatabase.h"
#include "tableAsciiNames.h"

class mainWindow : public QWidget
{
	Q_OBJECT

	public:
		mainWindow(const QString &log_name, QWidget *parent = 0);
		void reloadGroups();
		void center(QWidget *widget, int w, int h);

	public slots:
		void s_start();
		void s_stat();
		void s_option();

	private:
		QGridLayout *grid_gb;
		QGroupBox *gb_vokgru;
		QComboBox *cb_tense;
		QPushButton *pb_voption;
		QPushButton *pb_stat;
		QPushButton *pb_start;
		QStringList sl_vokgru;
		QStringList sl_tense;
		QList<QRadioButton*> lrb_vokgru;
		QScrollArea	*sa_scrollArea;
		QString name;
		abfrage *Abfrage;
};

#endif
