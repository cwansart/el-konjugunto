//	Quit Button einbaun

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

#include "mainWindow.h"

void mainWindow::center(QWidget *widget, int w, int h)
{
	int x, y;
	int screenWidth;
	int screenHeight;

	QDesktopWidget *desktop = QApplication::desktop();

	screenWidth = desktop->width();
	screenHeight = desktop->height();

	x = (screenWidth - w) / 2;
	y = (screenHeight - h) / 2;

	widget->move(x, y);
}

mainWindow::mainWindow(const QString &log_name, QWidget *parent)
	:	QWidget(parent)
{
	int width  = 430;
	int height = 350;

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

	//------------- Variablen & Objekte --------------------
	name = log_name;
	vocDatabase vocDB;

	pb_voption 	= new QPushButton("Optionen", this);
	pb_stat 		= new QPushButton("Statistik", this);
	pb_start 		= new QPushButton("Start", this);

	QPixmap image(QLatin1String(":/images/el_konjungunto_logo_0.3.png"));

	QLabel *l_titel 	= new QLabel(this);
	l_titel->setPixmap(image);
	l_titel->setScaledContents(true);

	QLabel *l_tense	= new QLabel("Zeit:", this);
	l_tense->setAlignment(Qt::AlignRight | Qt::AlignCenter);

	/********************************************
	*	Füllt die GroupBox mit den Vokabelgruppen	*
	********************************************/

	//---------------- ScrollArea ---------------------------
	//	ScrollArea für die GroupBox um bei zu vielen Einträgen Scrollen zu können
	sa_scrollArea = new QScrollArea(this);
	sa_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	sa_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

	//---------------- GroupBox -----------------------------
	grid_gb = new QGridLayout(this);
	gb_vokgru = new QGroupBox(sa_scrollArea);
	gb_vokgru->setFlat(true);

	sl_vokgru = vocDB.getGroupNames();

	reloadGroups();

	/************************************
	*	Füllt die ComboBox mit den Zeiten	*
	************************************/
	cb_tense = new QComboBox(this);
	sl_tense = vocDB.getAvailableTenses();
	cb_tense->addItems(tan::convert_new_all(sl_tense));

	/**************
	*	Grid-Layout	*
	**************/
	QGridLayout *grid = new QGridLayout(this);
	grid->setSpacing(15);

	grid->addWidget(l_titel, 0, 0, 2, 5);
	//	Die ScrollArea muss eingebunden werden, da die GroupBox
	//	nur ein Teil der ScrollArea darstellt
	grid->addWidget(sa_scrollArea, 2, 0, 4, 2);
	grid->addWidget(l_tense, 2, 2, 1, 1);
	grid->addWidget(cb_tense, 2, 3, 1, 2);
	grid->addWidget(pb_voption, 5, 2, 1, 1);
	grid->addWidget(pb_stat, 5, 3, 1, 1);
	grid->addWidget(pb_start, 5, 4, 1, 1);

	setLayout(grid);

	/**********************
	*				Connects			*
	**********************/

	connect(pb_start, SIGNAL(clicked()), SLOT(s_start()));
	connect(pb_stat, SIGNAL(clicked()), SLOT(s_stat()));
	connect(pb_voption, SIGNAL(clicked()), SLOT(s_option()));

	center(this, width, height);
}

void mainWindow::reloadGroups()
{
	qDebug() << "Reloading groups";

	delete grid_gb;
	delete gb_vokgru;

	gb_vokgru = new QGroupBox(sa_scrollArea);
	gb_vokgru->setFlat(true);
	grid_gb = new QGridLayout(this);

	lrb_vokgru.clear();

	for(int i = 0; i < sl_vokgru.count(); i++)
	{
		QRadioButton *rb_r = new QRadioButton(sl_vokgru[i], this);
		lrb_vokgru.append(rb_r);
		grid_gb->addWidget(lrb_vokgru[i], i, 0);
	}

	gb_vokgru->setLayout(grid_gb);
	sa_scrollArea->setWidget(gb_vokgru);

	lrb_vokgru[0]->setChecked(true);
}

void mainWindow::s_start()
{
	vocDatabase vocDB;
	QString group;

	for(int i = 0; i < lrb_vokgru.count(); i++)
	{
		if(lrb_vokgru[i]->isChecked())
			group = lrb_vokgru[i]->text();
	}

	QString tense = cb_tense->currentText();

	tense = tan::convert_old_one(tense);

	QStringList sl_ranvoc;
	//	p zählt wie oft i bei doppelten Vokabeln schon verringert wurde
	//	ab 10mal wir aufgeört i zu verringern die Wahrscheinlichkeit,
	//	dass nach weiteren max. 10mal immer noch kein 5 Vokabel gruppen
	//	ermittelt wurden ist gering genug um es bei 10 zu belassen
	int p = 0;

	for(int i = 0; i < 5/*HIER COUNT VARIABLE EINFÜGEN*/; i++)
	{
		QStringList sl_temp = vocDB.getRandomVoc(tense, group);

		bool multi = 0;

		if(sl_temp.isEmpty())
		{
			qDebug() << "Fehler: Keine Vokabeln vorhanden";
			QMessageBox::warning(this, tr("Fehler"), tr("Keine Vokabeln für diese Zeit vorhanden."));
			return;
		}

		qDebug() << sl_temp;

		for(int j = 0; j < sl_ranvoc.count(); j++)
		{
			if(sl_ranvoc[j] == sl_temp[0])
			{
				multi = 1;
				if(p <= 10)
				{
					p++;
					i--;
				}
			}
		}

		if(multi == 0)
		{
			sl_ranvoc.append(sl_temp);
			sl_ranvoc.append("|=START=|");
		}
	}

	qDebug() << sl_ranvoc;
	Abfrage = new abfrage(sl_ranvoc, cb_tense->currentText(), name, this);
	Abfrage->setWindowTitle("Konjungunto - Abfrage");
	Abfrage->show();
}

void mainWindow::s_stat()
{
	statistik statistik(name);
	statistik.setWindowTitle("Konjungunto - Statistik");
	statistik.exec();
}

void mainWindow::s_option()
{
	optionen optionen;
	optionen.setWindowTitle("Konjungunto - Optionen");
	optionen.exec();

	//	WTF? kA wieso aber es funktioniert nur so
	vocDatabase vocDB;
	sl_vokgru = vocDB.getGroupNames();
	qDebug() << sl_vokgru;
	reloadGroups();
}
