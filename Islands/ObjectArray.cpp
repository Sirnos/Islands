#include "ObjectArray.hpp"

ObjectArray::ObjectArray()
{
}

ObjectArray::~ObjectArray()
{
    ObjectVector.clear();
}

void ObjectArray::loadObjects()
{
    rapidxml::file<> File("Objects.xml");
    rapidxml::xml_document<> document;
    document.parse<0>(File.data());

    for (rapidxml::xml_node<>* node = document.first_node(); node != NULL;
         node = node->next_sibling())
    {
        ObjectsGraphicsFile = node->first_attribute()->value();
        for (rapidxml::xml_node<>* nNode = node->first_node(); nNode != NULL;
             nNode = nNode->next_sibling())
        {
            for (rapidxml::xml_node<>* nnNode = nNode->first_node(); nnNode != NULL;
                 nnNode = nnNode->next_sibling())
            {
                rapidxml::xml_node<>* Node3 = nnNode->first_node();

                std::string ObjName = nnNode->first_attribute()->value();
                bool isDestructible = false;
                std::pair<std::string, unsigned> IFDESTROYED;
                sf::Vector2f size;
                sf::IntRect textureCords;
                sf::FloatRect collisionBox;

                for (Node3; Node3 != NULL; Node3 = Node3->next_sibling())
                {
                    std::string parametrName(Node3->name());
                    rapidxml::xml_node<>* Node4 = Node3->first_node();
                    if (parametrName == "ISDESTRUCTIBLE")
                    {
                        if (Node4->value() == "1")
                        {
                            isDestructible = true;
                        }
                    }

                    for (Node4; Node4 != NULL; Node4 = Node4->next_sibling())
                    {
                        std::string nodeName = Node4->name();
                        if (nodeName == "DROP")
                        {
                            IFDESTROYED.first = std::string(Node4->value());
                        }
                        else if (nodeName == "AMOUNT")
                        {
                            IFDESTROYED.second
                                = static_cast<unsigned>(std::stoi(std::string(Node4->value())));
                        }
                        else if (nodeName == "X")
                        {
                            size.x = static_cast<float>(std::stoi(std::string(Node4->value())));
                        }
                        else if (nodeName == "Y")
                        {
                            size.y = static_cast<float>(std::stoi(std::string(Node4->value())));
                        }
                        else if (nodeName == "TOP")
                        {
                            if (parametrName == "COLLISION")
                                collisionBox.top
                                    = static_cast<float>(std::stoi(std::string(Node4->value())));
                            else if (parametrName == "GRAPHICS")
                                textureCords.top = std::stoi(std::string(Node4->value()));
                        }
                        else if (nodeName == "LEFT")
                        {
                            if (parametrName == "COLLISION")
                                collisionBox.left
                                    = static_cast<float>(std::stoi(std::string(Node4->value())));
                            else if (parametrName == "GRAPHICS")
                                textureCords.left = std::stoi(std::string(Node4->value()));
                        }
                        else if (nodeName == "WIDTH")
                        {
                            if (parametrName == "COLLISION")
                                collisionBox.width
                                    = static_cast<float>(std::stoi(std::string(Node4->value())));
                            else if (parametrName == "GRAPHICS")
                                textureCords.width = std::stoi(std::string(Node4->value()));
                        }
                        else if (nodeName == "HEIGHT")
                        {
                            if (parametrName == "COLLISION")
                                collisionBox.height
                                    = static_cast<float>(std::stoi(std::string(Node4->value())));
                            else if (parametrName == "GRAPHICS")
                                textureCords.height = std::stoi(std::string(Node4->value()));
                        }
                    }
                }
                Object temp{ ObjName, size, textureCords, collisionBox, isDestructible,
                    IFDESTROYED.first, IFDESTROYED.second };
                ObjectVector.push_back(temp);
            }
        }
    }
}

bool ObjectArray::isObjectExist(std::string objectID)
{
    for (auto & i : ObjectVector)
    {
        if (i.getID() == objectID)
        {
            return true;
        }
    }

    return false;
}

Object ObjectArray::getObject(std::string objectID)
{
    for (auto i : ObjectVector)
    {
        if (i.getID() == objectID)
        {
            return i;
        }
    }
    return Object();
}

std::string ObjectArray::getObjectsGraphicsFile()
{
    return ObjectsGraphicsFile;
}
