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

#include "statistik.h"

statistik::statistik(const QString &name, QWidget *parent)
	:	QDialog(parent)
{
	int width  = 300;
	int height = 170;

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

	userDatabase userDB;

	QStringList stats = userDB.getUserStatistic(name);

	QPixmap image(QLatin1String(":/images/el_konjungunto_logo_0.3.png"));

	l_titel = new QLabel(this);
	l_stat = new QLabel(("<b>Statistik für: " + name + "</b>"), this);
	l_beafra = new QLabel("Beantwortete Fragen", this);
	l_richtig = new QLabel("Richtig", this);
	l_falsch = new QLabel("Falsch", this);
	l_beafra_n = new QLabel(stats[0], this);
	l_richtig_n = new QLabel(stats[1], this);
	l_falsch_n = new QLabel(stats[2], this);

	l_stat->setAlignment(Qt::AlignCenter);
	l_titel->setPixmap(image);
	l_titel->setScaledContents(true);

	QGridLayout *grid = new QGridLayout(this);

	grid->addWidget(l_titel, 0, 0, 1, 6);
	grid->addWidget(l_stat, 1, 0, 1, 6);
	grid->addWidget(l_beafra, 2, 0, 1, 3);
	grid->addWidget(l_richtig, 2, 3, 1, 1);
	grid->addWidget(l_falsch, 2, 5, 1, 1);
	grid->addWidget(l_beafra_n, 3, 0, 1, 3);
	grid->addWidget(l_richtig_n, 3, 3, 1, 1);
	grid->addWidget(l_falsch_n, 3, 5, 1, 1);

//	setLayout(grid);
//	center(this, width, height);
}

statistik::statistik(const QString &beafra_n, const QString &richtig_n, const QString &falsch_n, QWidget *parent)
	:	QDialog(parent)
{
	int width  = 300;
	int height = 170;

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


	QPixmap image(QLatin1String(":/images/el_konjungunto_logo_0.3.png"));

	l_titel = new QLabel(this);
	l_stat = new QLabel(("<b>Statistik der Abfrage </b>"), this);
	l_beafra = new QLabel("Beantwortete Fragen", this);
	l_richtig = new QLabel("Richtig", this);
	l_falsch = new QLabel("Falsch", this);
	l_beafra_n = new QLabel(beafra_n, this);
	l_richtig_n = new QLabel(richtig_n, this);
	l_falsch_n = new QLabel(falsch_n, this);

	l_stat->setAlignment(Qt::AlignCenter);
	l_titel->setPixmap(image);
	l_titel->setScaledContents(true);

	QGridLayout *grid = new QGridLayout(this);

	grid->addWidget(l_titel, 0, 0, 1, 6);
	grid->addWidget(l_stat, 1, 0, 1, 6);
	grid->addWidget(l_beafra, 2, 0, 1, 3);
	grid->addWidget(l_richtig, 2, 3, 1, 1);
	grid->addWidget(l_falsch, 2, 5, 1, 1);
	grid->addWidget(l_beafra_n, 3, 0, 1, 3);
	grid->addWidget(l_richtig_n, 3, 3, 1, 1);
	grid->addWidget(l_falsch_n, 3, 5, 1, 1);

//	setLayout(grid);
//	center(this, width, height);
}
