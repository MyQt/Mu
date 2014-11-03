/*
 * Copyright (C) Kreogist Dev Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#include <QLabel>
#include <QBoxLayout>

#include "knlocalemanager.h"

#include "knmusicplaylistemptyhint.h"

KNMusicPlaylistEmptyHint::KNMusicPlaylistEmptyHint(QWidget *parent) :
    KNDropProxyContainer(parent)
{
    //Set properties.
    setAutoFillBackground(true);

    //Set palette.
    QPalette pal=palette();
    pal.setColor(QPalette::Window, QColor(0x35, 0x35, 0x35));
    pal.setColor(QPalette::WindowText, QColor(0xA0, 0xA0, 0xA0));
    setPalette(pal);

    //Initial the layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::LeftToRight, this);
    setLayout(mainLayout);

    mainLayout->addStretch();

    QBoxLayout *contentLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                             mainLayout->widget());
    contentLayout->setContentsMargins(0,0,0,0);
    contentLayout->setSpacing(20);
    mainLayout->addLayout(contentLayout);

    //Initial the empty hint icon.
    QLabel *emptyHintIcon=new QLabel(this);
    emptyHintIcon->setPixmap(QPixmap(":/plugin/music/common/emptyicon.png"));
    contentLayout->addWidget(emptyHintIcon,
                          0,
                          Qt::AlignRight | Qt::AlignVCenter);

    //Initial the hint layout.
    QBoxLayout *hintLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                          contentLayout->widget());
    contentLayout->addLayout(hintLayout);
    mainLayout->addStretch();

    //Initial the hint text lable.
    m_hintText=new QLabel(this);
    //Set palette.
    pal=m_hintText->palette();
    pal.setColor(QPalette::WindowText, QColor(0xA0, 0xA0, 0xA0));
    m_hintText->setPalette(pal);
    //Set the property.
    m_hintText->setWordWrap(true);
    m_hintText->setSizePolicy(QSizePolicy::Expanding,
                              QSizePolicy::Expanding);
    //Add to layout.
    hintLayout->addWidget(m_hintText);

    //Connect retranslate signal.
    connect(KNLocaleManager::instance(), &KNLocaleManager::requireRetranslate,
            this, &KNMusicPlaylistEmptyHint::retranslate);
    //Retranslate.
    retranslate();
}

void KNMusicPlaylistEmptyHint::retranslate()
{
    m_hintText->setText(tr("Songs you add to playlists and playlists you "
                           "import appear in Playlist.\n"
                           "To create a playlist, drag your songs here."));
}