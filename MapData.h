#pragma once

// Allow any format of input files (XML, PBF, ...)
#include <osmium/io/any_input.hpp>

// We want to use the handler interface
#include <osmium/handler.hpp>

// Utility class gives us access to memory usage information
#include <osmium/util/memory.hpp>

// For osmium::apply()
#include <osmium/visitor.hpp>

#include <iostream>
#include <vector>
#include <map>

#include <QPainter>
#include <QPainterPath>
#include <QDebug>

#include <osmium/handler.hpp>
#include <osmium/index/index.hpp>
#include <osmium/index/map/dummy.hpp>
#include <osmium/index/node_locations_map.hpp>
#include <osmium/osm/location.hpp>
#include <osmium/osm/node.hpp>
#include <osmium/osm/node_ref.hpp>
#include <osmium/osm/types.hpp>
#include <osmium/osm/way.hpp>


class MapData : public osmium::handler::Handler
{
    std::map<osmium::object_id_type, osmium::Location> m_nodeMap;

//    osmium::Location m_smallectLoc = osmium::Location(30.6114013, 46.3426662);
//    osmium::Location m_biggestLoc = osmium::Location(30.8313753, 46.6291187);
    osmium::Location m_smallectLoc = osmium::Location(30.6114013, 46.3426662);
    osmium::Location m_biggestLoc = osmium::Location(30.8313753, 46.6291187);

    double           m_scaleX = 1800.0 / (m_biggestLoc.lon() - m_smallectLoc.lon());
    double           m_scaleY = 950.0 / (m_biggestLoc.lat() - m_smallectLoc.lat());;
public:
    std::vector<QPainterPath> m_pathVector;

    MapData()
    {}

    void node(const osmium::Node& node) noexcept
    {
        if (node.location())
        {
            m_nodeMap[node.id()] = node.location();
        }
    }


    void way(osmium::Way& way) noexcept
    {
        const char* name = way.tags()["name"];
        if (name == nullptr) return;

        if (const auto* type = way.tags()["disused:route"]; type != nullptr && strcmp(type, "ferry") == 0 )
        {
            return;
        }
        if (const auto* type = way.tags()["route"]; type != nullptr && strcmp(type, "ferry") == 0 )
        {
            return;
        }

        qDebug() << "----------" << name << ' ' << way.nodes().size();
        auto& tags = way.tags();
        for( auto& tag : tags )
        {
            qDebug() << tag.key() << ' ' << tag.value();
        }

        osmium::Location prevLocation;
        m_pathVector.emplace_back();

        for (const auto& node_ref : way.nodes())
        {
            if (const auto it = m_nodeMap.find(node_ref.ref()); it != m_nodeMap.end())
            {
                double lon = (it->second.lon() - m_smallectLoc.lon()) * m_scaleX;
                double lat = (it->second.lat() - m_smallectLoc.lat()) * m_scaleY;

                if (prevLocation)
                {
                    m_pathVector.back().lineTo(lon, lat);
                    m_pathVector.back().moveTo(lon, lat);
                }
                else
                {
                    m_pathVector.back().moveTo(lon, lat);
                }

                prevLocation = it->second;
            }
        }
    }

    void relation(const osmium::Relation& relation) noexcept
    {
        if (const auto* type = relation.tags()["type"]; type == nullptr || strcmp(type, "route") != 0)
        {
            return;
        }

        const char* name = relation.tags()["name"];
        if ( name != nullptr )
        {
            qDebug() << "----------" << name << ' ' << relation.members().size();
            auto& tags = relation.tags();
            for( auto& tag : tags )
            {
                qDebug() << tag.key() << ' ' << tag.value();
            }
        }

        osmium::Location prevLocation;
        m_pathVector.emplace_back();

        for (const auto& node_ref : relation.members())
        {
            const auto& object = node_ref.get_object();
            if (!object.is_compatible_to(osmium::item_type::node))
            {
                continue;
            }

            if (const auto it = m_nodeMap.find(node_ref.ref()); it != m_nodeMap.end())
            {
                double lon = (it->second.lon() - m_smallectLoc.lon()) * m_scaleX;
                double lat = (it->second.lat() - m_smallectLoc.lat()) * m_scaleY;

                if (prevLocation)
                {
                    m_pathVector.back().lineTo(lon, lat);
                    m_pathVector.back().moveTo(lon, lat);
                }
                else
                {
                    m_pathVector.back().moveTo(lon, lat);
                }

                prevLocation = it->second;
            }
        }
    }

    void read();
};


