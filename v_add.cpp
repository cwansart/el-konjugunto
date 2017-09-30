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

#include "v_add.h"

v_addWindow::v_addWindow(QWidget *parent)
	:	QDialog(parent)
{
	int width  = 270;
	int height = 390;

	resize(width, height);
	// Sorgt dafür, dass das Fenster nicht verkleinert werden kann
	// (max|min Weite, max|min Höhe)
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
	grid->setSpacing(15);

	QLabel *l_infinitiv = new QLabel("Infinitiv:", this);

	QLabel *l_tense = new QLabel("Zeit:", this);
	l_tense->setAlignment(Qt::AlignRight);

	QLabel *l_group = new QLabel("Gruppe:", this);
	l_group->setAlignment(Qt::AlignRight);

	QLabel *l_form = new QLabel("Person:", this);

	le_name = new QLineEdit(this);

	cb_tense		= new QComboBox();
	cb_group	= new QComboBox();


	grid->addWidget(l_infinitiv, 0, 0, 1, 1);
	grid->addWidget(le_name, 0, 1, 1, 3);
	grid->addWidget(l_tense, 1, 0, 1, 1);
	grid->addWidget(cb_tense, 1, 1, 1, 3);
	grid->addWidget(l_group, 2, 0, 1, 1);
	grid->addWidget(cb_group, 2, 1, 1, 3);
	grid->addWidget(l_form, 3, 0, 1, 1);

	for(int i = 3; i < 9; i++)
	{
		QLabel *l_person;
		if(i <= 5)
			l_person = new QLabel((QString::number(i - 2)+ ". P SG"), this);
		else
			l_person = new QLabel((QString::number(i - 5)+ ". P  PL"), this);

		l_person->setAlignment(Qt::AlignRight | Qt::AlignBottom);
		QLineEdit	*le_l = new QLineEdit(this);
		lle_input.append(le_l);
		grid->addWidget(l_person, i + 1, 0, 1, 1);
		grid->addWidget(lle_input[i - 3], i + 1, 1, 1, 3);
	}

	//	So landet man mit TAB nicht erst bei OK und Abbrechen - 23:59Uhr
	QPushButton *pb_ok = new QPushButton("OK", this);
	QPushButton *pb_cancel = new QPushButton("Abbrechen", this);

	grid->addWidget(pb_ok, 10, 2, 1, 1);
	grid->addWidget(pb_cancel, 10, 3, 1, 1);

	cb_tense->addItems(tan::convert_new_all(vocDB.getAvailableTenses()));

	cb_group->addItems(vocDB.getGroupNames());

	setLayout(grid);

	connect(pb_ok, SIGNAL(clicked()), SLOT(s_apply()));
	connect(pb_cancel, SIGNAL(clicked()), this, SLOT(reject()));
	connect(cb_tense, SIGNAL(currentIndexChanged(QString)), this, SLOT(s_tense(QString)));

	setWindowTitle("Vokabel hinzufügen");
}

void v_addWindow::s_tense(QString tense)
{
	for(int i = 0; i < 6; i++)
	{
		lle_input[i]->setEnabled(true);
		lle_input[i]->setText("");
	}

	if(tense == "Imperativo")
	{
		lle_input[0]->setEnabled(false);
		lle_input[0]->setText("Nicht vorhanden");
	}

	if(tense == "Perfecto" || tense == "Pluscuamperfecto" || tense == "Gerundio")
	{
		for(int i = 1; i < 6; i++)
		{
			lle_input[i]->setEnabled(false);
			lle_input[i]->setText("Nicht vorhanden");
		}
	}
}

void v_addWindow::s_apply()
{
	bool full = 1;
	QStringList add;

	QString tense = cb_tense->currentText();

	tense = tan::convert_old_one(tense);

	add << tense << le_name->text() << cb_group->currentText();

	for(int i = 0; i < 6; i++)
	{
		if(lle_input[i]->text() == "Nicht vorhanden")
			continue;

		if(lle_input[i]->text() == "" || lle_input[i]->text() == "Bitte ausfüllen")
		{
			lle_input[i]->setText("Bitte ausfüllen");
			full = 0;
		}

		if(full == 1)
			add.append(lle_input[i]->text());
	}

	if(full == 0)
		return;
	else
	{
		qDebug() << add;
		vocDB.addVoc(add);
		accept();
	}
}
