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

#ifndef STATISTIK_H
#define STATISTIK_H

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
// ----------------------
#include <QGridLayout>
// ----------------------
#include "userDatabase.h"

class statistik : public QDialog
{
	Q_OBJECT

	public:
		statistik(const QString &name, QWidget *parent = 0);
		statistik(const QString &beafra_n, const QString &richtig_n, const QString &falsch_n, QWidget *parent = 0);

	public slots:

	private:
		QLabel *l_titel;
		QLabel *l_stat;
		QLabel *l_beafra;
		QLabel *l_richtig;
		QLabel *l_falsch;
		QLabel *l_beafra_n;
		QLabel *l_richtig_n;
		QLabel *l_falsch_n;

};

#endif
