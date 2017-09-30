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

#include "v_mod.h"

v_modWindow::v_modWindow(QWidget *parent)
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

	cb_tense = new QComboBox();
	cb_infinitiv = new QComboBox();
	cb_group = new QComboBox();

	QLabel *l_form = new QLabel("Person:", this);

	grid->addWidget(l_group, 0, 0, 1, 1);
	grid->addWidget(cb_group, 0, 1, 1, 3);
	grid->addWidget(l_tense, 1, 0, 1, 1);
	grid->addWidget(cb_tense, 1, 1, 1, 3);
	grid->addWidget(l_infinitiv, 2, 0, 1, 1);
	grid->addWidget(cb_infinitiv, 2, 1, 1, 3);
	grid->addWidget(l_form, 3, 0, 1, 1);

	for(int i = 4; i < 10; i++)
	{
		QLabel *l_person;
		if(i <= 6)
			l_person = new QLabel((QString::number(i - 3)+ ". P SG"), this);
		else
			l_person = new QLabel((QString::number(i - 6)+ ". P  PL"), this);

		l_person->setAlignment(Qt::AlignRight | Qt::AlignBottom);
		QLineEdit	*l = new QLineEdit(this);
		lle_input.append(l);
		grid->addWidget(l_person, i, 0, 1, 1);
		grid->addWidget(lle_input[i - 4], i, 1, 1, 3);
	}

	//	So landet man mit TAB nicht erst bei OK und Abbrechen - 23:59Uhr
	QPushButton *pb_ok = new QPushButton("OK", this);
	QPushButton *pb_cancel = new QPushButton("Abbrechen", this);

	grid->addWidget(pb_ok, 10, 2, 1, 1);
	grid->addWidget(pb_cancel, 10, 3, 1, 1);

	setLayout(grid);

	cb_group->addItems(vocDB.getGroupNames());
	s_load_tense(cb_group->currentText());

	QString tense = cb_tense->currentText();

	tense = tan::convert_old_one(tense);

	s_load_infinitiv(tense);

	connect(pb_ok, SIGNAL(clicked()), SLOT(s_apply()));
	connect(pb_cancel, SIGNAL(clicked()), this, SLOT(reject()));
	connect(cb_group, SIGNAL(currentIndexChanged(QString)), this, SLOT(s_load_tense(QString)));
	connect(cb_tense, SIGNAL(currentIndexChanged(QString)), this, SLOT(s_load_infinitiv(QString)));
	connect(cb_infinitiv, SIGNAL(currentIndexChanged(QString)), this, SLOT(s_load_voc(QString)));

	setWindowTitle("Vokabel bearbeiten");
}

void v_modWindow::s_apply()
{
	QString tense = cb_tense->currentText();

	tense = tan::convert_old_one(tense);

	for(int i = 0; i < sl_vocs.count(); i++)
	{
		if(lle_input[i]->text() != sl_vocs[i])
		{
			qDebug() << "Change: " + sl_vocs[i] + " to " + lle_input[i]->text() + " in time " + tense;
			vocDB.updateVoc(tense, cb_infinitiv->currentText(), lle_input[i]->text(), (i + 1));
		}
	}
	accept();
}

void v_modWindow::s_load_tense(QString group)
{
	cb_tense->clear();

	cb_tense->addItems(tan::convert_new_all(vocDB.getTenses(group)));
}

void v_modWindow::s_load_infinitiv(QString tense)
{
	tense = tan::convert_old_one(tense);

	cb_infinitiv->clear();
	cb_infinitiv->addItems(vocDB.getVoc(cb_group->currentText(), tense));
	qDebug() << cb_group->currentText() << tense;
}

void v_modWindow::s_load_voc(QString infinitiv)
{
	if(infinitiv.isEmpty())
		return;
	
	sl_vocs.clear();
	for(int i = 0; i < lle_input.count(); i++)
	{
		lle_input[i]->setEnabled(true);
		lle_input[i]->clear();
	}

	QString tense = cb_tense->currentText();

	tense = tan::convert_old_one(tense);

	sl_vocs = vocDB.getVoc(cb_group->currentText(), tense, infinitiv);
	qDebug() << cb_group->currentText() << tense << infinitiv;

	if(sl_vocs.count() == 5)
		lle_input[0]->setEnabled(false);
	else if(sl_vocs.count() == 1)
	{
		for(int i = 1; i < 6; i++)
			lle_input[i]->setEnabled(false);
	}

	int j = 0;

	for(int i = 0; i < 6; i++)
	{
		if(lle_input[i]->isEnabled())
		{
			lle_input[i]->setText(sl_vocs[j]);
			j++;
		}
	}
}
