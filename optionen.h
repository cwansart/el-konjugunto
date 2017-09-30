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

#ifndef OPTIONEN_H
#define OPTIONEN_H

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
#include <QPixmap>
#include <QGroupBox>
#include <QComboBox>
// ----------------------
#include <QGridLayout>
// ----------------------
#include "v_add.h"
#include "v_mod.h"
#include "vocDatabase.h"

class optionen : public QDialog
{
	Q_OBJECT

	public:
		optionen(QWidget *parent = 0);

	public slots:
		void s_vadd();
		void s_vmod();
		void s_gadd();
		void s_gdel();

	private:
		QLabel *l_titel;
		QLabel *l_option;
		QPushButton *pb_import;
		QPushButton *pb_export;
		QPushButton *pb_add;
		QPushButton *pb_mod;
		QPushButton *pb_gadd;
		QPushButton *pb_gdel;
};

class addgru : public QDialog
{
	Q_OBJECT

	public:
		addgru(QWidget *parent = 0);

	public slots:
		void s_add();

	private:
		QLabel *l_name;
		QLineEdit *le_name;
		QPushButton *pb_ok;
		QPushButton *pb_cancel;
};

class delgru : public QDialog
{
	Q_OBJECT

	public:
		delgru(QWidget *parent = 0);

	public slots:
		void s_del();

	private:
		QLabel *l_name;
		QComboBox *cb_name;
		QPushButton *pb_ok;
		QPushButton *pb_cancel;
};

#endif
