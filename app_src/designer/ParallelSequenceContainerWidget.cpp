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

#include "ParallelSequenceContainerWidget.hpp"
#include "ui_ParallelSequenceContainerWidget.h"

#include <QDialog>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>

#include <fstream>

#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>

ParallelSequenceContainerWidget::ParallelSequenceContainerWidget(PSequenceMeta & PSequences, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::ParallelSequencesContainerWidget),
	m_ParallelSequences(PSequences)
{
	ui->setupUi(this);
    ui->NameEdit->setText(QString::fromStdString(PSequences.getName()));
}

ParallelSequenceContainerWidget::~ParallelSequenceContainerWidget()
{
	delete ui;
}

void ParallelSequenceContainerWidget::setName(const QString & name)
{
	ui->NameEdit->setText(name);
    m_ParallelSequences.setName(name.toStdString());
}

QString ParallelSequenceContainerWidget::getName() const
{
    return QString::fromStdString(m_ParallelSequences.getName());
}


PiGPIOSequence::ParallelSequences const & ParallelSequenceContainerWidget::getSequences() const noexcept
{
    return m_ParallelSequences.getPSeq();
}

void ParallelSequenceContainerWidget::on_EditorButton_clicked()
{
    emit onEdit(m_ParallelSequences.accessPSeq());
}

void ParallelSequenceContainerWidget::on_SaveButton_clicked()
{
    QString Path = QFileDialog::getExistingDirectory(this, "Ordner auswaehlen...");

    if (!Path.isEmpty())
    {
        std::ofstream ofile;
            ofile.open(QString(Path + "/" + ui->NameEdit->text() + ".pseq").toStdString());

        if (!ofile.good())
        {
            QMessageBox::warning(this, "Fehler", "Konnte nicht speichern!");
        }

        cereal::JSONOutputArchive oarchive(ofile);
        oarchive(m_ParallelSequences);

        //TODO Hier müsste auch nochmal der Stream überprüft werden
    }
}

void ParallelSequenceContainerWidget::on_LoadButton_clicked()
{
    QString Path = QFileDialog::getOpenFileName(this, "Datei auswaehlen...");
    if (Path.isEmpty())
    {
        return;
    }

    std::ifstream ifile;
        ifile.open(Path.toStdString());

    if (!ifile.good())
    {
        QMessageBox::warning(this, "Fehler", "Konnte es nicht laden...");
    }

    cereal::JSONInputArchive iarchive(ifile);
    iarchive(m_ParallelSequences);

    ui->NameEdit->setText(QString::fromStdString(m_ParallelSequences.getName()));

    //TODO Nochmal stream überprüfen, ob wirklich alles gelesen werden konnte
}

void ParallelSequenceContainerWidget::on_NameEdit_editingFinished()
{
    m_ParallelSequences.setName(ui->NameEdit->text().toStdString());
}
