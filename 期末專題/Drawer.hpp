#ifndef DRAWER_HPP
#define DRAWER_HPP
template <typename TGameRendererConfig>
class Drawer
{
public:
    using CellPosition = Feis::CellPosition;
    using Direction = Feis::Direction;

    Drawer(sf::RenderWindow *window) : window_(window)
    {
        if (!font_.loadFromFile("arial.ttf"))
        {
            std::cout << "Error loading font" << std::endl;
        }
    }

    void Clear()
    {
        window_->clear(sf::Color::Black);
    }

    void Display()
    {
        window_->display();
    }

    void DrawBorder(CellPosition cellPosition)
    {
        sf::Vector2f topLeft =
            GetBorderTopLeft() +
            sf::Vector2f(cellPosition.col, cellPosition.row) * static_cast<float>(TGameRendererConfig::kCellSize);

        sf::RectangleShape rectangle(
            sf::Vector2f(
                TGameRendererConfig::kCellSize - 2 * TGameRendererConfig::kBorderSize,
                TGameRendererConfig::kCellSize - 2 * TGameRendererConfig::kBorderSize));

        rectangle.setOutlineColor(sf::Color(60, 60, 60));
        rectangle.setOutlineThickness(TGameRendererConfig::kBorderSize);
        rectangle.setFillColor(sf::Color::Black);
        rectangle.setPosition(topLeft + sf::Vector2f(TGameRendererConfig::kBorderSize, TGameRendererConfig::kBorderSize));
        window_->draw(rectangle);
    }

    void DrawText(
        std::string str,
        unsigned int characterSize,
        sf::Color color,
        sf::Vector2f position,
        Direction direction = Direction::kTop)
    {
        sf::Text text;
        text.setFont(font_);
        text.setString(str);
        text.setCharacterSize(characterSize);
        text.setFillColor(color);

        sf::FloatRect rect = text.getLocalBounds();
        text.setOrigin(rect.left + rect.width / 2.0f, rect.top + rect.height / 2.0f);
        text.setPosition(position);
        text.setRotation(90 * static_cast<int>(direction));
        window_->draw(text);
    }

    void DrawText(
        std::string str,
        unsigned int characterSize,
        sf::Color color,
        CellPosition cellPosition,
        Direction direction = Direction::kTop)
    {
        DrawText(str, characterSize, color, GetCellCenter(cellPosition), direction);
    }

    void DrawRectangle(CellPosition cellPosition, sf::Color color)
    {
        sf::RectangleShape rectangle(sf::Vector2f(TGameRendererConfig::kCellSize, TGameRendererConfig::kCellSize));
        rectangle.setOrigin(rectangle.getLocalBounds().width / 2, rectangle.getLocalBounds().height / 2);
        rectangle.setFillColor(color);
        rectangle.setPosition(GetCellCenter(cellPosition));
        DrawShape(rectangle);
    }

    void DrawTriangle(
        sf::Vector2f center,
        Direction direction,
        sf::Color color)
    {
        sf::ConvexShape triangle;
        triangle.setPointCount(3);
        triangle.setPoint(0, sf::Vector2f(0, 0));
        triangle.setPoint(1, sf::Vector2f(TGameRendererConfig::kCellSize, 0));
        triangle.setPoint(2, sf::Vector2f(TGameRendererConfig::kCellSize, TGameRendererConfig::kCellSize));
        triangle.setOrigin(triangle.getLocalBounds().width / 2, triangle.getLocalBounds().height / 2);
        triangle.rotate((static_cast<int>(direction) + 1) * 90);
        triangle.setFillColor(color);
        triangle.setPosition(center);
        DrawShape(triangle);
    }

    void DrawTriangle(CellPosition cellPosition, Direction direction, sf::Color color)
    {
        DrawTriangle(GetCellCenter(cellPosition), direction, color);
    }

    void DrawCircle(sf::Vector2f center, float radius, sf::Color color)
    {
        sf::CircleShape circle(radius);
        circle.setOrigin(circle.getLocalBounds().width / 2, circle.getLocalBounds().height / 2);
        circle.setFillColor(color);
        circle.setPosition(center);
        circle.setOutlineColor(sf::Color(60, 60, 60));
        circle.setOutlineThickness(2);
        DrawShape(circle);
    }

    void DrawArrow(CellPosition cellPosition, Feis::Direction direction)
    {
        int offset = 2;
        sf::ConvexShape arrow(6);
        arrow.setPoint(0, sf::Vector2f(0, 0));
        arrow.setPoint(1, sf::Vector2f(-2 * offset, offset - TGameRendererConfig::kCellSize / 2));
        arrow.setPoint(2, sf::Vector2f(0, offset - TGameRendererConfig::kCellSize / 2));
        arrow.setPoint(3, sf::Vector2f(2 * offset, 0));
        arrow.setPoint(4, sf::Vector2f(0, TGameRendererConfig::kCellSize / 2 - offset));
        arrow.setPoint(5, sf::Vector2f(-2 * offset, TGameRendererConfig::kCellSize / 2 - offset));
        arrow.rotate((static_cast<int>(direction) + 3) * 90);
        arrow.setFillColor(sf::Color(60, 60, 60));
        arrow.setPosition(GetCellCenter(cellPosition));
        DrawShape(arrow);
    }

    void DrawShape(const sf::Shape &s)
    {
        window_->draw(s);
    }

    sf::Vector2f GetCellCenter(CellPosition cellPosition)
    {
        return GetCellTopLeft(cellPosition) + sf::Vector2f(TGameRendererConfig::kCellSize / 2, TGameRendererConfig::kCellSize / 2);
    }
    sf::Vector2f GetCellTopLeft(CellPosition cellPosition)
    {
        return GetBorderTopLeft() +
               sf::Vector2f(cellPosition.col, cellPosition.row) * static_cast<float>(TGameRendererConfig::kCellSize);
    }
    sf::Vector2f GetBorderTopLeft()
    {
        return sf::Vector2f(TGameRendererConfig::kBoardLeft, TGameRendererConfig::kBoardTop);
    }

private:
    sf::RenderWindow *window_;
    sf::Font font_;
};
#endif