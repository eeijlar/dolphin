/***************************************************************************
 *   Copyright (C) 2011 by Peter Penz <peter.penz19@gmail.com>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA            *
 ***************************************************************************/

#ifndef KITEMLISTHEADERWIDGET_H
#define KITEMLISTHEADERWIDGET_H

#include "dolphin_export.h"

#include <QGraphicsWidget>
#include <QHash>
#include <QList>

class KItemModelBase;

/**
 * @brief Widget the implements the header for KItemListView showing the currently used roles.
 *
 * The widget is an internal API, the user of KItemListView may only access the
 * class KItemListHeader.
 */
class DOLPHIN_EXPORT KItemListHeaderWidget : public QGraphicsWidget
{
    Q_OBJECT

public:
    explicit KItemListHeaderWidget(QGraphicsWidget* parent = nullptr);
    ~KItemListHeaderWidget() override;

    void setModel(KItemModelBase* model);
    KItemModelBase* model() const;

    void setAutomaticColumnResizing(bool automatic);
    bool automaticColumnResizing() const;

    void setColumns(const QList<QByteArray>& roles);
    QList<QByteArray> columns() const;

    void setColumnWidth(const QByteArray& role, qreal width);
    qreal columnWidth(const QByteArray& role) const;

    /**
     * Sets the column-width that is required to show the role unclipped.
     */
    void setPreferredColumnWidth(const QByteArray& role, qreal width);
    qreal preferredColumnWidth(const QByteArray& role) const;

    void setOffset(qreal offset);
    qreal offset() const;

    qreal minimumColumnWidth() const;

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;

signals:
    /**
     * Is emitted if the width of a visible role has been adjusted by the user with the mouse
     * (no signal is emitted if KItemListHeader::setVisibleRoleWidth() is invoked).
     */
    void columnWidthChanged(const QByteArray& role,
                            qreal currentWidth,
                            qreal previousWidth);

    /**
     * Is emitted if the user has released the mouse button after adjusting the
     * width of a visible role.
     */
    void columnWidthChangeFinished(const QByteArray& role,
                                   qreal currentWidth);

    /**
     * Is emitted if the position of the column has been changed.
     */
    void columnMoved(const QByteArray& role, int currentIndex, int previousIndex);

    /**
     * Is emitted if the user has changed the sort order by clicking on a
     * header item. The sort order of the model has already been adjusted to
     * the current sort order. Note that no signal will be emitted if the
     * sort order of the model has been changed without user interaction.
     */
    void sortOrderChanged(Qt::SortOrder current, Qt::SortOrder previous);

    /**
     * Is emitted if the user has changed the sort role by clicking on a
     * header item. The sort role of the model has already been adjusted to
     * the current sort role. Note that no signal will be emitted if the
     * sort role of the model has been changed without user interaction.
     */
    void sortRoleChanged(const QByteArray& current, const QByteArray& previous);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent* event) override;

private slots:
    void slotSortRoleChanged(const QByteArray& current, const QByteArray& previous);
    void slotSortOrderChanged(Qt::SortOrder current, Qt::SortOrder previous);

private:
    void paintRole(QPainter* painter,
                   const QByteArray& role,
                   const QRectF& rect,
                   int orderIndex,
                   QWidget* widget = nullptr) const;

    void updatePressedRoleIndex(const QPointF& pos);
    void updateHoveredRoleIndex(const QPointF& pos);
    int roleIndexAt(const QPointF& pos) const;
    bool isAboveRoleGrip(const QPointF& pos, int roleIndex) const;

    /**
     * Creates a pixmap of the role with the index \a roleIndex that is shown
     * during moving a role.
     */
    QPixmap createRolePixmap(int roleIndex) const;

    /**
     * @return Target index of the currently moving visible role based on the current
     *         state of m_movingRole.
     */
    int targetOfMovingRole() const;

    /**
     * @return x-position of the left border of the role \a role.
     */
    qreal roleXPosition(const QByteArray& role) const;

private:
    enum RoleOperation
    {
        NoRoleOperation,
        ResizeRoleOperation,
        MoveRoleOperation
    };

    bool m_automaticColumnResizing;
    KItemModelBase* m_model;
    qreal m_offset;
    QList<QByteArray> m_columns;
    QHash<QByteArray, qreal> m_columnWidths;
    QHash<QByteArray, qreal> m_preferredColumnWidths;

    int m_hoveredRoleIndex;
    int m_pressedRoleIndex;
    RoleOperation m_roleOperation;
    QPointF m_pressedMousePos;

    struct MovingRole
    {
        QPixmap pixmap;
        int x;
        int xDec;
        int index;
    } m_movingRole;
};

#endif


