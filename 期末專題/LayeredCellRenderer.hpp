#ifndef LAYERED_CELL_RENDERER_HPP
#define LAYERED_CELL_RENDERER_HPP
#include "Drawer.hpp"
#include "CellRendererFirstPassVisitor.hpp"
#include "CellRendererSecondPassVisitor.hpp"
#include "CellRendererThirdPassVisitor.hpp"

template<typename TGameRendererConfig>
class LayeredCellRenderer
{
public:
    using IGameInfo = Feis::IGameInfo;
    using CellPosition = Feis::CellPosition;

    void RenderPassOne(
        const IGameInfo &info,
        Drawer<TGameRendererConfig> &renderer, 
        CellPosition position) const
    {
        auto& layeredCell = info.GetLayeredCell(position);

        auto foreground = layeredCell.GetForeground();
        auto background = layeredCell.GetBackground();

        if (foreground)
        {
            CellRendererFirstPassVisitor<TGameRendererConfig> cellRenderer(&info, &renderer, position, background.get());
            foreground->Accept(&cellRenderer);
            return;
        }

        if (background)
        {
            CellRendererFirstPassVisitor<TGameRendererConfig> cellRenderer(&info, &renderer, position, background.get());
            background->Accept(&cellRenderer);
            return;
        }
        
        renderer.DrawBorder(position);
    }

    void RenderPassTwo(
        const IGameInfo &info,
        Drawer<TGameRendererConfig> &drawer,
        CellPosition cellPosition) const
    {
        auto& layeredCell = info.GetLayeredCell(cellPosition);

        auto foreground = layeredCell.GetForeground();

        if (foreground)
        {
            CellRendererSecondPassVisitor<TGameRendererConfig> cellRenderer(&info, &drawer, cellPosition);
            foreground->Accept(&cellRenderer);
        }
    }

    void RenderPassThree(
        const IGameInfo &info,
        Drawer<TGameRendererConfig> &drawer,
        CellPosition cellPosition) const
    {
        auto& layeredCell = info.GetLayeredCell(cellPosition);

        auto foreground = layeredCell.GetForeground();
        if (foreground)
        {
            CellRendererThirdPassVisitor<TGameRendererConfig> cellRenderer(&info, &drawer, cellPosition);
            foreground->Accept(&cellRenderer);
        }
    }
};
#endif