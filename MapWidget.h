#pragma once

#include <QPainter>
#include <QWidget>
#include <QPainterPath>

#include "MapData.h"

class MapWidget : public QWidget
{
    MapData m_mapData;

    double m_xOffset;
    double m_yOffset;

    double m_xScale;
    double m_yScale;
public:
    MapWidget(QWidget *parent = nullptr) : QWidget(parent)
    {}

    void setupWidget()
    {
        m_mapData.read();
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        Q_UNUSED(event);

        QPainter painter(this);
        QPen pen(Qt::yellow);

        pen.setWidth(1);
        painter.setPen(pen);
        painter.translate(0, height());
        painter.scale(1.0, -1.0);

        for (const auto& path : m_mapData.m_pathVector)
        {
            painter.drawPath(path);
        }
    }
};
