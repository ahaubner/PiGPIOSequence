/*
 *   Copyright (C) 2016 Anton Haubner <anton.haubner@outlook.de>
 *
 *   This file is part of the PiGPIOSequenceWidgets library.
 *
 *   PiGPIOSequenceWidgets is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU Affero General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   PiGPIOSequenceWidgets is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Affero General Public License for more details.
 *
 *   You should have received a copy of the GNU Affero General Public License
 *   along with PiGPIOSequenceWidgets.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <PiGPIOSequence/Extensions/Widgets/SequenceListItem.hpp>
#include "ui_SequenceListItem.h"

#include <QDialog>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>

#include <PiGPIOSequence/Extensions/Widgets/PinList.hpp>

#include <fstream>
#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>

namespace PiGPIOSequence
{
	namespace Extensions
	{
		namespace Widgets
		{
            SequenceListItem::SequenceListItem(Sequence & seq, QWidget * parent)
				:	QWidget(parent),
                    ui(new Ui::SequenceListItem),
					m_Sequence(seq)
			{
				ui->setupUi(this);
			}

            SequenceListItem::~SequenceListItem()
			{
				delete ui;
			}

            void SequenceListItem::setName(const QString & name)
			{
				ui->NameEdit->setText(name);
			}

            QString SequenceListItem::getName() const
			{
				return ui->NameEdit->text();
			}

            Sequence const & SequenceListItem::getSequence() const noexcept
			{
				return m_Sequence;
			}

            void SequenceListItem::on_EditorButton_clicked()
			{
                QDialog * Dialog = new QDialog(this);
                    Dialog->setAttribute(Qt::WA_DeleteOnClose); //Das sollte sich um das delete kümmern

                auto * Widget = new PinList(m_Sequence, Dialog);

                QVBoxLayout * Layout = new QVBoxLayout(Dialog);
                    Layout->addWidget(Widget);
                    Dialog->setLayout(Layout);
                    Dialog->setFixedSize(512, 512);
                    Dialog->setWindowTitle(ui->NameEdit->text() + " Editor");

					Dialog->exec();
			}

            void SequenceListItem::on_SaveButton_clicked()
			{
                QString Path = QFileDialog::getExistingDirectory(this, "Ordner auswaehlen...");

                if (!Path.isEmpty())
                {
                    std::ofstream ofile;
                        ofile.open(QString(Path + "/" + ui->NameEdit->text() + ".seq").toStdString());

                    if (!ofile.good())
                    {
                        QMessageBox::warning(this, "Fehler", "Konnte nicht speichern!");
                    }

                    cereal::JSONOutputArchive oarchive(ofile);
                    oarchive(m_Sequence);
                }
			}

            void SequenceListItem::on_LoadButton_clicked()
			{
                const QString Path = QFileDialog::getOpenFileName(this, "Datei auswaehlen...");
                if (Path.isEmpty())
                {
                    return;
                }

                {
                    std::ifstream ifile;
                        ifile.open(Path.toStdString());

                    if (!ifile.good())
                    {
                        QMessageBox::warning(this, "Fehler", "Konnte es nicht laden...");
                    }

                    cereal::JSONInputArchive iarchive(ifile);

                    iarchive(m_Sequence);
                }

                const QFileInfo info(Path);
                ui->NameEdit->setText(info.completeBaseName());
			}
		}
	}
}
