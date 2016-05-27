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

#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include <QMessageBox>

#include <PiGPIOSequence/Extensions/Widgets/SequenceMixer.hpp>

#include <IPFind.hpp>

#include "InfoDialog.hpp"
#include "ParallelSequenceContainerWidget.hpp"
#include "ParallelSequencesEditDialog.hpp"

using namespace std;
using namespace PiGPIOSequence::Extensions::Networking;

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
    ui(new Ui::MainWindow),
	m_Client(true),
    m_IsConnected(false),
    m_ClientWork(false)

{
	ui->setupUi(this);

	//connect(&m_MixerWidget, SIGNAL(SequenceMixed()), this, SLOT(newMix()));
    //connect(ui->ConnectButton, &QPushButton::clicked, this, &MainWindow::on_ConnectButton_clicked);

    m_Client.onConnectionClosed([this]()
    {
        setConnected(false);
    });

    m_Client.onSequenceFinished([this]()
    {
        ui->ClientStatusLabel->setText("Sequenz beendet");
    });

    connect(&m_UpdateTimer, SIGNAL(timeout()), this, SLOT(update()));
    m_UpdateTimer.start(0);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::onPSeqEdit(PiGPIOSequence::ParallelSequences & PSeq)
{
	std::unique_ptr<ParallelSequencesEditDialog> Diag(new ParallelSequencesEditDialog(PSeq, this));

	Diag->exec();
}

void MainWindow::on_AddSequenceButton_clicked()
{
    m_ParallelSequences.emplace_back();

    updatePSeqListUI();
}

void MainWindow::on_RemoveSequenceButton_clicked()
{
	QList<QListWidgetItem *> List = ui->ParallelSequencesListWidget->selectedItems();

    for (int i = 0; i < List.size(); ++i)
	{
        const auto pIndex = ui->ParallelSequencesListWidget->row(List.at(i));

        m_ParallelSequences.erase(m_ParallelSequences.begin() + static_cast<ParallelSequencesVector::difference_type>(pIndex));
	}

    updatePSeqListUI();
}

void MainWindow::on_SendButton_clicked()
{
	QList<QListWidgetItem *> List = ui->ParallelSequencesListWidget->selectedItems();
	if (List.size() < 1)
	{
		QMessageBox::information(this, "Problem", "Es wurde nichts zum Senden ausgewaehlt");
		return;
	}

	auto Item = List.at(0);
	auto Widget = static_cast<ParallelSequenceContainerWidget *> (ui->ParallelSequencesListWidget->itemWidget(Item));
    //Widget->getSequence().swapConfigSyntax(m_NetSyntax);

    auto seq_ptr = std::make_shared<PiGPIOSequence::Sequence>(PiGPIOSequence::combine_sequences(Widget->getSequences()));

    m_Client.sendSequence(*seq_ptr, [this, seq_ptr, Widget](Client::Error error)
    {
        if (!error)
        {
			QMessageBox::information(this, "Erfolg", "Sequenzen erfolgreich an Pi Server verschickt!");
            refreshStatus();
        }

        else
        {
			QMessageBox::warning(this, "Fehler", "Konnte Sequenzen nicht verschicken!");
        }
    });
}

void MainWindow::on_InfoButton_clicked()
{
	auto * Diag = new InfoDialog(this);

	Diag->exec();

	delete Diag;
}

void MainWindow::on_FindIPButton_clicked()
{
    IPFind::Client Finder;

	std::string address;

	try
	{
		address = Finder.find(4124);
	}

	catch(...)
	{}

	if (!address.empty())
	{
		ui->IPInput->setText(QString::fromStdString(address));
		return;
	}

	QMessageBox::critical(this, "Fehler", "Konnte keinen Pi mit entsprechender Serversoftware im Netzwerk finden.");

}

void MainWindow::on_IPInput_returnPressed()
{
    on_ConnectButton_clicked();
}

void MainWindow::setConnected(bool onOff)
{
    m_ClientWork = onOff;

    ui->ClientStatusLabel->setText((onOff ?
                                        "Verbunden" :
                                        "Getrennt"));

    ui->ConnectionFrame->setEnabled(onOff);

    m_IsConnected = onOff;

	if (!onOff)
	{
		m_Client.reset();
	}
}

void MainWindow::refreshStatus()
{
    if (m_IsConnected)
    {
        m_Client.requestStatus([this](Client::ServerStatus status, Client::Error const & error)
        {
            if (error)
            {
                QMessageBox::warning(this, "Fehler", "Fehler bei Statusabfrage!");
            }

            else
            {
                switch (status)
                {
                    case Client::ServerStatus::Normal:
                        ui->ClientStatusLabel->setText("Normal");
                    break;

                    case Client::ServerStatus::RunningSequence:
                        ui->ClientStatusLabel->setText("Durchläuft Sequenz");
                    break;

                    default:
                        ui->ClientStatusLabel->setText("Unbekannt");
                    break;
                }
            }
        });
    }
}

void MainWindow::updatePSeqListUI()
{
    ui->ParallelSequencesListWidget->clear();

    for (ParallelSequencesVector::size_type i = 0; i < m_ParallelSequences.size(); ++i)
    {
        addPSeqListItem_impl(m_ParallelSequences[i]);
    }
}

void MainWindow::addPSeqListItem_impl(PSequenceMeta & pSeqMeta)
{
    QListWidgetItem * Item = new QListWidgetItem(ui->ParallelSequencesListWidget);
    ParallelSequenceContainerWidget * Widget = new ParallelSequenceContainerWidget(pSeqMeta, ui->ParallelSequencesListWidget);
    Item->setSizeHint(Widget->minimumSizeHint());
    ui->ParallelSequencesListWidget->addItem(Item);
    ui->ParallelSequencesListWidget->setItemWidget(Item, Widget);

    connect(Widget, &ParallelSequenceContainerWidget::onEdit, this, &MainWindow::onPSeqEdit);
}

void MainWindow::update()
{
    if (m_ClientWork)
    {
        m_Client.run_one();
    }
}

void MainWindow::on_ConnectButton_clicked()
{
	if (!m_ClientWork)
	{
		m_Client.reset();
	}

    m_ClientWork = true;

    //IP wurde eingegeben, man soll sich verbinden
    m_Client.disconnect([this](Client::Error error)
    {
        if(!error)
        {
            setConnected(false);
        }

		m_Client.reset();
		m_ClientWork = true;

        m_Client.connect(ui->IPInput->text().toStdString(), [this](Client::Error error)
        {
            if (error)
            {
                setConnected(false);
                QMessageBox::warning(this, "Fehler", "Konnte nicht zur Adresse verbinden!");
            }

            else
            {
                setConnected(true);
            }
        });
    });
}


void MainWindow::on_StatusRequestButton_clicked()
{
    refreshStatus();
}

void MainWindow::on_SequenceStopButton_clicked()
{
    if (m_IsConnected)
    {
        m_Client.stopSequence([this](Client::Error error)
        {
            if (error)
            {
                QMessageBox::warning(this, "Fehler", "Konnte keine Sequenz stoppen!");
            }
        });
    }
}

void MainWindow::on_DisconnectButton_clicked()
{
	m_Client.disconnect([this](Client::Error error)
	{
		if(!error)
		{
			setConnected(false);
		}

		else
		{
			QMessageBox::warning(this, "Fehler", "Fehler beim Trennen der Verbindung");
		}
	});
}
