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

#ifndef PIGPIOSEQUENCE_EXTENSIONS_WIDGETS_PINSELECTOR_HPP
#define PIGPIOSEQUENCE_EXTENSIONS_WIDGETS_PINSELECTOR_HPP

#include <QWidget>
#include <map>
#include <vector>

#include <PiGPIOSequence.hpp>

namespace PiGPIOSequence
{
    namespace Extensions
    {
        namespace Widgets
        {
            namespace Ui
            {
                class PinSelector;
            }

            class PinSelector
                :   public QWidget
            {
                    Q_OBJECT

                public:
                    enum Pins
                        :   Utils::PinNumber
                    {
                        V3_3_N0 = 0,
                        V5_N0 = 1,
                        GPIO2 = 2,
                        V5_N1 = 3,
                        GPIO3 = 4,
                        GROUND_N0 = 5,
                        GPIO4 = 6,
                        GPIO14 = 7,
                        GROUND_N1 = 8,
                        GPIO15 = 9,
                        GPIO17 = 10,
                        GPIO18 = 11,
                        GPIO27 = 12,
                        GROUND_N2 = 13,
                        GPIO22 = 14,
                        GPIO23 = 15,
                        V3_3_N1 = 16,
                        GPIO24 = 17,
                        GPIO10 = 18,
                        GROUND_N3 = 19,
                        GPIO9 = 20,
                        GPIO25 = 21,
                        GPIO11 = 22,
                        GPIO8 = 23,
                        GROUND_N4 = 24,
                        GPIO7 = 25,

                        ID_SD = 26,
                        ID_SC = 27,
                        GPIO5 = 28,
                        GROUND_N5 = 29,
                        GPIO6 = 30,
                        GPIO12 = 31,
                        GPIO13 = 32,
                        GROUND_N6 = 33,
                        GPIO19 = 34,
                        GPIO16 = 35,
                        GPIO26 = 36,
                        GPIO20 = 37,
                        GROUND_N7 = 38,
                        GPIO21 = 39
                    };

					using PinList = std::vector<Pins>;
					using PinMap = std::map<Pins, bool>;
					using GPIONumList = std::vector<Utils::PinNumber>;

                    explicit PinSelector(QWidget * parent = nullptr);
                    virtual ~PinSelector() override;

                    PinMap const & getPinStates() const noexcept;
                    void getSelectedPins(PinList & ReturnList) const;
                    void getSelectedGPIOs(GPIONumList & ReturnList) const;

                signals:
                    void pinsChanged();

                private slots:
                    void on_V3_3_N0_clicked();

                    void on_GPIO2_clicked();

                    void on_V5_N0_clicked();

                    void on_V5_N1_clicked();

                    void on_GPIO3_clicked();

                    void on_GPIO14_clicked();

                    void on_GROUND_N0_clicked();

                    void on_GPIO4_clicked();

                    void on_GROUND_N1_clicked();

                    void on_GPIO15_clicked();

                    void on_GPIO17_clicked();

                    void on_GPIO18_clicked();

                    void on_GPIO27_clicked();

                    void on_GROUND_N2_clicked();

                    void on_GPIO22_clicked();

                    void on_GPIO23_clicked();

                    void on_V3_3_N1_clicked();

                    void on_GPIO24_clicked();

                    void on_GPIO10_clicked();

                    void on_GROUND_N3_clicked();

                    void on_GPIO9_clicked();

                    void on_GPIO25_clicked();

                    void on_GPIO11_clicked();

                    void on_GPIO8_clicked();

                    void on_GROUND_N4_clicked();

                    void on_GPIO7_clicked();

                    void on_ID_SD_clicked();

                    void on_ID_SC_clicked();

                    void on_GPIO5_clicked();

                    void on_GROUND_N5_clicked();

                    void on_GPIO6_clicked();

                    void on_GPIO12_clicked();

                    void on_GPIO13_clicked();

                    void on_GROUND_N6_clicked();

                    void on_GPIO19_clicked();

                    void on_GPIO16_clicked();

                    void on_GPIO26_clicked();

                    void on_GPIO20_clicked();

                    void on_GROUND_N7_clicked();

                    void on_GPIO21_clicked();

                private:
                    void PinClicked(Pins Pin, bool state);

                    Ui::PinSelector * ui;

                    PinMap m_PinMap;
            };
        }
    }
}

#endif
