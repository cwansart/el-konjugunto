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

#include "login.h"

login::login(QWidget *parent)
	:	QWidget(parent)
{
	int width  = 420;
	int height = 260;

	resize(width, height);
	//	Sorgt dafür, dass das Fenster nicht verkleinert werden kann
	//	(max|min Weite, max|min Höhe)
	setMaximumSize(width, height);
	setMinimumSize(width, height);

	//	um den Maximierungs Button abzuschalten
	Qt::WindowFlags flags;
	flags = Qt::Window
	      | Qt::CustomizeWindowHint
	      | Qt::WindowTitleHint
	      | Qt::WindowMinimizeButtonHint
	      | Qt::WindowCloseButtonHint;
	setWindowFlags(flags);

	l_titel = new QLabel(this);
	/*
	l_login = new QLabel("<h1><center>Login</center></h1>", this);
	l_name 	= new QLabel("Name:", this);
	l_pass	= new QLabel("Passwort:", this);
	*/
	l_login = new QLabel(tr("<h1><center>Login</center></h1>"), this);
	l_name 	= new QLabel(tr("Name:"), this);
	l_pass	= new QLabel(tr("Password:"), this);
	

	le_name = new QLineEdit(this);
	le_pass = new QLineEdit(this);

	le_pass->setEchoMode(QLineEdit::Password);

	/*
	pb_login = new QPushButton("Login", this);
	pb_acc = new QPushButton("Neues Konto", this);
	*/
	pb_login = new QPushButton(tr("Login"), this);
	pb_acc = new QPushButton(tr("New user"), this);

	QPixmap image(QLatin1String(":/images/el_konjungunto_logo_0.3.png"));

	l_titel->setPixmap(image);
	l_titel->setScaledContents(true);

	//	Zentriert den Titel
	l_titel->setAlignment(Qt::AlignCenter);

	QGridLayout *layout = new QGridLayout;
	QHBoxLayout *buttonsLayout = new QHBoxLayout;
	
	layout->addWidget(l_titel, 0, 0, 1, -1);
	layout->addWidget(l_login, 1, 0, 1, -1);

	layout->addWidget(l_name, 2, 0);
	layout->addWidget(le_name, 2, 1);

	layout->addWidget(l_pass, 3, 0);
	layout->addWidget(le_pass, 3, 1);

	buttonsLayout->addWidget(pb_login);
	buttonsLayout->addWidget(pb_acc);
	
	layout->addLayout(buttonsLayout, 4, 1);
	setLayout(layout);

	connect(pb_login, SIGNAL(clicked()), SLOT(s_login()));
	connect(pb_acc, SIGNAL(clicked()), SLOT(s_reg()));

}

void login::s_login()
{
	userDatabase userDB;

	QString name = le_name->text();
	QString pass = le_pass->text();
//	QString pass(QCryptographicHash::hash(pass.toUtf8(),QCryptographicHash::Md5));

	if(userDB.checkUserData(name, pass))
	{
		//	Aktiviert den Start-Button
		pb_login->setEnabled(false);
		pb_acc->setEnabled(false);
		le_pass->setEnabled(false);
		le_name->setEnabled(false);

		close();
		MainWindow = new mainWindow(le_name->text());
		//MainWindow->setWindowTitle("Konjungunto - Hauptmenu");
		MainWindow->setWindowTitle(tr("Konjungunto - Main menu"));
		MainWindow->show();
	}
	else
		QMessageBox::warning(this, tr("Error"), tr("Username or password are not correct."));
}

void login::s_reg()
{
	neukonto neukonto;
	neukonto.exec();
}


neukonto::neukonto(QWidget *parent)
	:	QDialog(parent)
{
	int width  = 300;
	int height = 150;

	resize(width, height);
	// Sorgt dafür, dass das Fenster nicht verkleinert werden kann
	// (max|min Weite, max|min Höhe)
	setMaximumSize(width, height);
	setMinimumSize(width, height);

	Qt::WindowFlags flags;
	flags = Qt::Window
	      | Qt::CustomizeWindowHint
	      | Qt::WindowTitleHint
	      | Qt::WindowMinimizeButtonHint
	      | Qt::WindowCloseButtonHint;
	setWindowFlags(flags);

	/*
	l_name = new QLabel("Name:", this);
	l_pass = new QLabel("Passwort:", this);
	l_pass2 = new QLabel("Passwort bestätigen:", this);
	*/
	le_name = new QLineEdit(this);
	le_pass = new QLineEdit(this);
	le_pass2 = new QLineEdit(this);
	/*
	pb_ok = new QPushButton("OK", this);
	pb_cancel  = new QPushButton("Cancel", this);
	*/
	
	l_name = new QLabel(tr("Name:"), this);
	l_pass = new QLabel(tr("Password:"), this);
	l_pass2 = new QLabel(tr("Password:"), this);
	
	pb_ok = new QPushButton(tr("OK"), this);
	pb_cancel  = new QPushButton(tr("Cancel"), this);


	le_pass->setEchoMode(QLineEdit::Password);
	le_pass2->setEchoMode(QLineEdit::Password);
	
	QGridLayout *layout = new QGridLayout;
	QHBoxLayout *buttonsLayout = new QHBoxLayout;

	layout->addWidget(l_name, 0, 0);
	layout->addWidget(le_name, 0, 1);

	layout->addWidget(l_pass, 1, 0);
	layout->addWidget(le_pass, 1, 1);
	
	layout->addWidget(l_pass2, 2, 0);
	layout->addWidget(le_pass2, 2 , 1);

	buttonsLayout->addWidget(pb_ok);
	buttonsLayout->addWidget(pb_cancel);

	layout->addLayout(buttonsLayout, 3, 0, 1, -1);
	setLayout(layout);
	//setWindowTitle("Neues Konto");
	setWindowTitle(tr("New user"));

	connect(pb_ok, SIGNAL(clicked()), SLOT(s_reg()));
	connect(pb_cancel, SIGNAL(clicked()), SLOT(reject()));
}

void neukonto::s_reg()
{
	QString name	= le_name->text();
	QString pass1 = le_pass->text();
	QString pass2 = le_pass2->text();
	
	if(name.isEmpty())
	{
		QMessageBox::warning(this, tr("Error"), tr("You've entered no username."));
		le_name->setFocus();
	}
	else if(pass1.isEmpty())
	{
		QMessageBox::warning(this, tr("Error"), tr("You need to enter a password to proceed."));
		le_pass->setFocus();
	}
	else if(pass2.isEmpty())
	{
		QMessageBox::warning(this, tr("Error"), tr("You need to retype your password."));
		le_pass2->setFocus();
	}
	else if(userDB.getUserInformation(name)[0] != "NULL")
	{
		QMessageBox::warning(this, tr("Error"), tr("Username already exists."));
		le_name->clear();
	}
	else if(pass1 != pass2 || pass1 == "")
	{
		//	1. tr = Titel text
		//	2. tr = Box text
		QMessageBox::warning(this, tr("Error"), tr("Passwords does not match."));
		le_pass->clear();
		le_pass2->clear();
	}
	else
	{
		userDB.addUser(name, pass1);
		//	Aktion bei übereinstimmenden Passwörtern
		accept();
		QMessageBox::information(this, tr("Account created"), tr("You user account was successfully created.<br><br>Have a lot of fun~"));
	}
}
