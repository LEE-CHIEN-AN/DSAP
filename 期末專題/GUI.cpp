#define USE_GUI
#include <fstream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "PDOGS.cpp"

#include "GameRenderer.hpp"

using namespace Feis;

struct GameRendererConfig
{
    static constexpr int kFPS = 30;
    static constexpr int kCellSize = 20;
    static constexpr int kBoardLeft = 20;
    static constexpr int kBoardTop = 60;
    static constexpr int kBorderSize = 1;
};

CellPosition GetMouseCellPosition(const sf::RenderWindow &window)
{
    const sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
    const sf::Vector2i relatedMousePosition =
        mousePosition - sf::Vector2i(GameRendererConfig::kBoardLeft, GameRendererConfig::kBoardTop);
    return {relatedMousePosition.y / GameRendererConfig::kCellSize,
            relatedMousePosition.x / GameRendererConfig::kCellSize};
}

class GamePlayerWithHistory : public IGamePlayer 
{
public:
    PlayerAction GetNextAction(const IGameInfo& info) override 
    {
        if (actions_.empty())
        {
            return {PlayerActionType::None, {0, 0}};
        }

        PlayerAction action = actions_.front();
        actions_.pop();
        return action;
    }

    void EnqueueAction(PlayerAction action)
    {
        actions_.push(action);
    }
private:
    std::queue<PlayerAction> actions_;
};

void Save(std::queue<PlayerAction> playerActionHistory, const std::string &filename)
{
    std::ofstream outFile(filename);
    while (!playerActionHistory.empty())
    {
        const PlayerAction &playerAction = playerActionHistory.front();
        outFile << playerAction.cellPosition.row << " " << playerAction.cellPosition.col << " " << static_cast<int>(playerAction.type) << std::endl;
        playerActionHistory.pop();
    }
}


int main(int, char **)
{
    sf::VideoMode mode = sf::VideoMode(1280, 1024);

    sf::RenderWindow window(mode, "DSAP Final Project", sf::Style::Close);

    window.setFramerateLimit(GameRendererConfig::kFPS);

    GamePlayer player;
    
    GameManager gameManager(&player, 1, 20);

    const std::map<sf::Keyboard::Key, PlayerActionType> playerActionKeyboardMap = {
        {sf::Keyboard::J, PlayerActionType::BuildLeftOutMiningMachine},
        {sf::Keyboard::I, PlayerActionType::BuildTopOutMiningMachine},
        {sf::Keyboard::L, PlayerActionType::BuildRightOutMiningMachine},
        {sf::Keyboard::K, PlayerActionType::BuildBottomOutMiningMachine},
        {sf::Keyboard::D, PlayerActionType::BuildLeftToRightConveyor},
        {sf::Keyboard::S, PlayerActionType::BuildTopToBottomConveyor},
        {sf::Keyboard::A, PlayerActionType::BuildRightToLeftConveyor},
        {sf::Keyboard::W, PlayerActionType::BuildBottomToTopConveyor},
        {sf::Keyboard::Num1, PlayerActionType::BuildTopOutCombiner},
        {sf::Keyboard::Num2, PlayerActionType::BuildRightOutCombiner},
        {sf::Keyboard::Num3, PlayerActionType::BuildBottomOutCombiner},
        {sf::Keyboard::Num4, PlayerActionType::BuildLeftOutCombiner},
        {sf::Keyboard::Backspace, PlayerActionType::Clear},
    };

    PlayerActionType playerActionType = PlayerActionType::BuildLeftToRightConveyor;

    GameRenderer<GameRendererConfig> gameRenderer(&window);

    std::queue<PlayerAction> playerActionHistory;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            /*if (event.type == sf::Event::MouseButtonReleased)
            {
                CellPosition mouseCellPosition = GetMouseCellPosition(window);

                if (IsWithinBoard(mouseCellPosition))
                {
                    if (event.mouseButton.button == sf::Mouse::Left)
                    {
                        auto playerAction = PlayerAction{playerActionType, mouseCellPosition};
                        player.EnqueueAction(playerAction);
                        playerActionHistory.push(playerAction);
                    }
                }
            }

            if (event.type == sf::Event::KeyReleased)
            {
                if (playerActionKeyboardMap.count(event.key.code))
                {
                    playerActionType = playerActionKeyboardMap.at(event.key.code);
                } 
                else if (event.key.code == sf::Keyboard::F4)
                {
                    Save(playerActionHistory, "gameplay.txt");
                }
            }*/
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        gameManager.Update();

        gameRenderer.Render(gameManager);  
    }
}