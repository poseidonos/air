/*
 *   MIT License
 *
 *   Copyright (c) 2021 Samsung Electronics Corporation
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in all
 *   copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *   SOFTWARE.
 */

#include "tool/tui/MoveHandler.h"

void
air::MoveHandler::HandleMove(EventData data, AConfig& tree)
{
    switch (data.type)
    {
        case air::EventType::MOVE_UP:
        {
            _MoveUp(tree);
            break;
        }
        case air::EventType::MOVE_DOWN:
        {
            _MoveDown(tree);
            break;
        }
        case air::EventType::MOVE_RIGHT:
        {
            _MoveRight(tree);
            break;
        }
        case air::EventType::MOVE_LEFT:
        {
            _MoveLeft(tree);
            break;
        }
        default:
        {
            break;
        }
    }
}

void
air::MoveHandler::_MoveUp(AConfig& tree)
{
    if (tree.group.begin()->second->here)
    {
        tree.pos_top = true;
        tree.pos_group = false;
        tree.pos_node = false;
        tree.pos_id = -1;
        tree.group.begin()->second->here = false;
    }
    else
    {
        bool now {false};
        bool done {false};
        air::AGroup* group {nullptr};
        air::ANode* node {nullptr};

        for (auto g = tree.group.rbegin(); g != tree.group.rend(); ++g)
        {
            if (!g->second->fold)
            {
                for (auto n = g->second->node.rbegin(); n != g->second->node.rend();
                     ++n)
                {
                    if (now)
                    {
                        tree.pos_top = false;
                        tree.pos_group = false;
                        tree.pos_node = true;
                        tree.pos_id = n->second->nid;
                        n->second->here = true;
                        done = true;
                        break;
                    }
                    else if (n->second->here)
                    {
                        now = true;
                        node = n->second;
                    }
                }
            }

            if (done)
            {
                break;
            }

            if (now)
            {
                tree.pos_top = false;
                tree.pos_group = true;
                tree.pos_node = false;
                tree.pos_id = g->second->gid;
                g->second->here = true;
                done = true;
                break;
            }
            else if (g->second->here)
            {
                now = true;
                group = g->second;
            }
        }

        if (done)
        {
            if (nullptr != group)
            {
                group->here = false;
            }
            if (nullptr != node)
            {
                node->here = false;
            }
        }
    }
}

void
air::MoveHandler::_MoveDown(AConfig& tree)
{
    if (tree.pos_top)
    {
        for (auto g : tree.group)
        {
            tree.pos_top = false;
            tree.pos_group = true;
            tree.pos_node = false;
            tree.pos_id = g.second->gid;
            g.second->here = true;
            break;
        }
    }
    else
    {
        bool now {false};
        bool done {false};
        air::AGroup* group {nullptr};
        air::ANode* node {nullptr};

        for (auto g : tree.group)
        {
            if (done)
            {
                break;
            }

            if (now)
            {
                tree.pos_top = false;
                tree.pos_group = true;
                tree.pos_node = false;
                tree.pos_id = g.second->gid;
                g.second->here = true;
                done = true;
                break;
            }
            else if (g.second->here)
            {
                now = true;
                group = g.second;
            }

            if (g.second->fold)
            {
                continue;
            }

            for (auto n : g.second->node)
            {
                if (now)
                {
                    tree.pos_top = false;
                    tree.pos_group = false;
                    tree.pos_node = true;
                    tree.pos_id = n.second->nid;
                    n.second->here = true;
                    done = true;
                    break;
                }
                else if (n.second->here)
                {
                    now = true;
                    node = n.second;
                }
            }
        }

        if (done)
        {
            if (nullptr != group)
            {
                group->here = false;
            }
            if (nullptr != node)
            {
                node->here = false;
            }
        }
    }
}

void
air::MoveHandler::_MoveRight(AConfig& tree)
{
    if (tree.pos_group)
    {
        for (auto g : tree.group)
        {
            if (tree.pos_id == g.second->gid)
            {
                g.second->fold = false;
            }
        }
    }
    else if (tree.pos_node)
    {
        for (auto g : tree.group)
        {
            for (auto n : g.second->node)
            {
                if (tree.pos_id == n.second->nid)
                {
                    n.second->fold = false;
                }
            }
        }
    }
}

void
air::MoveHandler::_MoveLeft(AConfig& tree)
{
    if (tree.pos_group)
    {
        for (auto g : tree.group)
        {
            if (tree.pos_id == g.second->gid)
            {
                g.second->fold = true;
            }
        }
    }
    else if (tree.pos_node)
    {
        for (auto g : tree.group)
        {
            for (auto n : g.second->node)
            {
                if (tree.pos_id == n.second->nid)
                {
                    n.second->fold = true;
                }
            }
        }
    }
}
