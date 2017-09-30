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

#ifndef LOGIN_H
#define LOGIN_H

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
#include <QTimer>
#include <QPixmap>
// #include <QCryptographicHash>
#include <QByteArray>
// ----------------------
#include <QBoxLayout>
#include <QGridLayout>

#include "mainWindow.h"
#include "userDatabase.h"

class login : public QWidget
{
	Q_OBJECT

	public:
		login(QWidget *parent = 0);

	public slots:
		void s_login();
		void s_reg();

	private:
		QLabel *l_titel;
		QLabel *l_login;
		QLabel *l_db;
		QLabel *l_name;
		QLabel *l_pass;
		QLineEdit *le_name;
		QLineEdit *le_pass;
		QPushButton *pb_login;
		QPushButton *pb_acc;
		mainWindow *MainWindow;
};

class neukonto : public QDialog
{
	Q_OBJECT

	public:
		neukonto(QWidget *parent = 0);

	public slots:
		void s_reg();

	private:
		QLabel *l_name;
		QLabel *l_pass;
		QLabel *l_pass2;
		QLineEdit *le_name;
		QLineEdit *le_pass;
		QLineEdit *le_pass2;
		QPushButton *pb_ok;
		QPushButton *pb_cancel;
		userDatabase userDB;
};

#endif
