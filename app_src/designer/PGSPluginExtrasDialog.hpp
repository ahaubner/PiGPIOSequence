/*
 *   Copyright (C) 2016 Anton Haubner <anton.haubner@outlook.de>
 *
 *   This file is part of the PiGPIOSequence Designer program.
 *
 *   PiGPIOSequence Designer is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU Affero General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   PiGPIOSequence Designer is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Affero General Public License for more details.
 *
 *   You should have received a copy of the GNU Affero General Public License
 *   along with PiGPIOSequence Designer.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PGSPLUGINEXTRASDIALOG_HPP
#define PGSPLUGINEXTRASDIALOG_HPP

#include <QDialog>

namespace Ui {
	class PGSPluginExtrasDialog;
}

class PGSPluginExtrasDialog : public QDialog
{
		Q_OBJECT

	public:
		explicit PGSPluginExtrasDialog(QWidget *parent = 0);
		~PGSPluginExtrasDialog();

	private:
		Ui::PGSPluginExtrasDialog *ui;
};

#endif // PGSPLUGINEXTRASDIALOG_HPP
