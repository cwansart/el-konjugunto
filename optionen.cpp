/****************************************************************************
**
**	Copyright (C) 2009 Viktor Baum, Timo Kikillus, Alex Meier,
**										 Florian Porrmann, Marcel Pokraka,
**										 Sebastian Steinhorst, Niklas Stumpe,
**										 Christian Wansart, Marcel Wiegand
**
**	This file is part of El Konjung�nto.
**
**	El Konjung�nto is free software: you can redistribute it and/or modify
**	it under the terms of the GNU General Public License as published by
**	the Free Software Foundation, either version 3 of the License, or
**	(at your option) any later version.
**
**	El Konjung�nto is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**	GNU General Public License for more details.
**
**	You should have received a copy of the GNU General Public License
**	along with El Konjung�nto.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#include "optionen.h"

optionen::optionen(QWidget *parent)
	:	QDialog(parent)
{
	int width  = 450;
	int height = 250;

	resize(width, height);
	// Sorgt daf�r, dass das Fenster nicht verkleinert werden kann
	// (max|min Weite, max|min H�he)
	setMaximumSize(width, height);
	setMinimumSize(width, height);

	// um den Maximierungs Button abzuschalten
	Qt::WindowFlags flags;
	flags = Qt::Window
	      | Qt::CustomizeWindowHint
	      | Qt::WindowTitleHint
	      | Qt::WindowMinimizeButtonHint
	      | Qt::WindowCloseButtonHint;
	setWindowFlags(flags);

	QPixmap image(QLatin1String(":/images/el_konjungunto_logo_0.3.png"));

	l_titel = new QLabel(this);
	l_option = new QLabel("<b>Vokabeloptionen</b>", this);

	pb_import = new QPushButton("Vokabeln importieren", this);
	pb_export = new QPushButton("Vokabeln exportieren", this);
	pb_add 	= new QPushButton("Vokabeln hinzuf�gen", this);
	pb_mod 	= new QPushButton("Vokabeln bearbeiten", this);
	pb_gadd	= new QPushButton("Gruppe hinzuf�gen", this);
	pb_gdel	= new QPushButton("Gruppe l�schen", this);

	pb_import->setEnabled(false);
	pb_export->setEnabled(false);
/*
	pb_add->setEnabled(false);
	pb_mod->setEnabled(false);
	pb_gadd->setEnabled(false);
	pb_gdel->setEnabled(false);
*/

	QGroupBox *gb_vok = new QGroupBox("Vokabeln", this);
	QGroupBox *gb_gru = new QGroupBox("Gruppen", this);

	l_option->setAlignment(Qt::AlignCenter);
	l_titel->setPixmap(image);
	l_titel->setScaledContents(true);

	QGridLayout *grid = new QGridLayout(this);
	grid->addWidget(l_titel, 0, 0, 2, 10);
	grid->addWidget(l_option, 2, 0, 1, 10);

	QGridLayout *v_grid = new QGridLayout(this);
	v_grid->addWidget(pb_import, 0, 0, 1, 2);
	v_grid->addWidget(pb_export, 0, 3, 1, 2);
	v_grid->addWidget(pb_add, 1, 0, 1, 2);
	v_grid->addWidget(pb_mod, 1, 3, 1, 2);
	gb_vok->setLayout(v_grid);

	QGridLayout *g_grid = new QGridLayout(this);
	g_grid->addWidget(pb_gadd, 0, 0, 1, 2);
	g_grid->addWidget(pb_gdel, 1, 0, 1, 2);
	gb_gru->setLayout(g_grid);

	grid->addWidget(gb_vok, 3, 0, 2, 7);
	grid->addWidget(gb_gru, 3, 7, 2, 3);

	setLayout(grid);

	connect(pb_add, SIGNAL(clicked()), SLOT(s_vadd()));
	connect(pb_mod, SIGNAL(clicked()), SLOT(s_vmod()));
	connect(pb_gadd, SIGNAL(clicked()), SLOT(s_gadd()));
	connect(pb_gdel, SIGNAL(clicked()), SLOT(s_gdel()));
}

void optionen::s_vadd()
{
	v_addWindow addWindow;
	addWindow.setWindowTitle("Vokabel hinzuf�gen");
	addWindow.exec();
}

void optionen::s_vmod()
{
	v_modWindow modWindow;
	modWindow.setWindowTitle("Vokabel bearbeiten");
	modWindow.exec();
}

void optionen::s_gadd()
{
	addgru gaddWindow;
	gaddWindow.setWindowTitle("Gruppe hinzuf�gen");
	gaddWindow.exec();
}

void optionen::s_gdel()
{
	delgru gdelWindow;
	gdelWindow.setWindowTitle("Gruppe l�schen");
	gdelWindow.exec();
}

addgru::addgru(QWidget *parent)
	:	QDialog(parent)
{
	int width  = 250;
	int height = 100;

	resize(width, height);
	// Sorgt daf�r, dass das Fenster nicht verkleinert werden kann
	// (max|min Weite, max|min H�he)
	setMaximumSize(width, height);
	setMinimumSize(width, height);

	// um den Maximierungs Button abzuschalten
	Qt::WindowFlags flags;
	flags = Qt::Window
	      | Qt::CustomizeWindowHint
	      | Qt::WindowTitleHint
	      | Qt::WindowMinimizeButtonHint
	      | Qt::WindowCloseButtonHint;
	setWindowFlags(flags);

	QGridLayout *grid = new QGridLayout(this);

	l_name		= new QLabel("Name:", this);
	le_name		= new QLineEdit(this);
	pb_ok			= new QPushButton("OK", this);
	pb_cancel	= new QPushButton("Cancel", this);


	grid->addWidget(l_name, 0, 0, 1, 1);
	grid->addWidget(le_name, 0, 1, 1, 2);
	grid->addWidget(pb_ok, 1, 1, 1, 1);
	grid->addWidget(pb_cancel, 1, 2, 1, 1);

	connect(pb_ok, SIGNAL(clicked()), SLOT(s_add()));
	connect(pb_cancel, SIGNAL(clicked()), SLOT(reject()));

	setWindowTitle("Gruppe hinzuf�gen");
}

void addgru::s_add()
{
	vocDatabase vocDB;
	vocDB.addGroup(le_name->text(), "");
	accept();
}

delgru::delgru(QWidget *parent)
	:	QDialog(parent)
{
	int width  = 250;
	int height = 100;

	resize(width, height);
	// Sorgt daf�r, dass das Fenster nicht verkleinert werden kann
	// (max|min Weite, max|min H�he)
	setMaximumSize(width, height);
	setMinimumSize(width, height);

	// um den Maximierungs Button abzuschalten
	Qt::WindowFlags flags;
	flags = Qt::Window
	      | Qt::CustomizeWindowHint
	      | Qt::WindowTitleHint
	      | Qt::WindowMinimizeButtonHint
	      | Qt::WindowCloseButtonHint;
	setWindowFlags(flags);

	QGridLayout *grid = new QGridLayout(this);

	l_name		= new QLabel("Name:", this);
	cb_name		= new QComboBox(this);
	pb_ok			= new QPushButton("OK", this);
	pb_cancel	= new QPushButton("Cancel", this);

	vocDatabase vocDB;
	cb_name->addItems(vocDB.getGroupNames());

	grid->addWidget(l_name, 0, 0, 1, 1);
	grid->addWidget(cb_name, 0, 1, 1, 2);
	grid->addWidget(pb_ok, 1, 1, 1, 1);
	grid->addWidget(pb_cancel, 1, 2, 1, 1);

	connect(pb_ok, SIGNAL(clicked()), SLOT(s_del()));
	connect(pb_cancel, SIGNAL(clicked()), SLOT(reject()));

	setWindowTitle("Gruppe hinzuf�gen");
}

void delgru::s_del()
{
	vocDatabase vocDB;
	vocDB.removeGroup(cb_name->currentText(), true);
	accept();
}
