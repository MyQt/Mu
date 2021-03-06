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
Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef KNMUSICSTOREPAGE_H
#define KNMUSICSTOREPAGE_H

#include <QVariant>

#include "knconnectionhandler.h"

#include "knmousesensewidget.h"

class KNMusicStoreBackend;
/*!
 * \brief The KNMusicStorePage class provides a page used for the abstract ports
 * of all the pages for the music store. These pages include home page, search
 * results, artist & album and single song.
 */
class KNMusicStorePage : public KNMouseSenseWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicStorePage widget.
     * \param parent The parent widget.
     */
    KNMusicStorePage(QWidget *parent = 0);

    /*!
     * \brief Get the name of the backend which provides the data for the
     * current page.
     * \return The backend name. If there's no backend for the current page, it
     * will return a null string.
     */
    QString backendName() const;

    /*!
     * \brief Save the metadata of the current page.
     * \return The current page metadata string.
     */
    QString metadata() const;

signals:
    /*!
     * \brief Set the navigator item text in the header text.
     * \param navigatorIndex The navigator item index.
     * \param caption The text of the navigator item.
     */
    void requireSetNavigatorItem(int navigatorIndex,
                                 const QString &caption);

    /*!
     * \brief When the page data is ready, this signal should be emit to show
     * the page.
     */
    void requireShowPage();

    /*!
     * \brief Ask the backend to fetch information about one artist.
     * \param metadata The song's metadata.
     */
    void requireShowArtist(QString metadata);

    /*!
     * \brief Ask the backend to fetch information about one album.
     * \param metadata The song's metadata.
     */
    void requireShowAlbum(QString metadata);

    /*!
     * \brief Ask the backend to fetch information about one single song.
     * \param metadata The song's metadata.
     */
    void requireShowSingleSong(QString metadata);

    /*!
     * \brief Ask the page container to update the page size.
     */
    void requireUpdateHeight();

    /*!
     * \brief Require to download one song file.
     * \param url The song url.
     * \param filename The saving file name of the song.
     * \param title The title of the song, this will display at the song list.
     */
    void requireDownload(const QString &url,
                         const QString &filename,
                         const QString &title);

public slots:
    /*!
     * \brief Clear all the data on the current page, reset the page to the very
     * beginning.
     */
    virtual void reset()=0;

    /*!
     * \brief Set the linked backend to the target page.
     * \param backend The backend object pointer.
     */
    virtual void setBackend(KNMusicStoreBackend *backend);

    /*!
     * \brief Set the label data at specific page.
     * \param labelIndex The index recorded in the util class.
     * \param value The value of the label.
     */
    virtual void setPageLabel(int labelIndex, const QVariant &value)=0;

    /*!
     * \brief Set the metadata of current page.
     * \param metadata The metadata of the backend.
     */
    void setMetadata(const QString &metadata);

private:
    QString m_backendName, m_metadata;
};

#endif // KNMUSICSTOREPAGE_H
