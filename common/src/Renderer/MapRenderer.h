/*
 Copyright (C) 2010-2017 Kristian Duske

 This file is part of TrenchBroom.

 TrenchBroom is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 TrenchBroom is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with TrenchBroom. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TrenchBroom_MapRenderer
#define TrenchBroom_MapRenderer

#include "Macros.h"
#include "IO/IO_Forward.h"
#include "Model/Model_Forward.h"
#include "Renderer/Renderer_Forward.h"

#include <map>
#include <memory>
#include <vector>

class Color;

namespace TrenchBroom {
    namespace View {
        // FIXME: Renderer should not depend on View
        class MapDocument;
        class Selection;
    }

    namespace Renderer {
        class MapRenderer {
        private:
            class SelectedBrushRendererFilter;
            class LockedBrushRendererFilter;
            class UnselectedBrushRendererFilter;

            using RendererMap = std::map<Model::Layer*, ObjectRenderer*>;

            std::weak_ptr<View::MapDocument> m_document;

            std::unique_ptr<ObjectRenderer> m_defaultRenderer;
            std::unique_ptr<ObjectRenderer> m_selectionRenderer;
            std::unique_ptr<ObjectRenderer> m_lockedRenderer;
            std::unique_ptr<EntityLinkRenderer> m_entityLinkRenderer;
        public:
            explicit MapRenderer(std::weak_ptr<View::MapDocument> document);
            ~MapRenderer();

            deleteCopyAndMove(MapRenderer)
        private:
            static std::unique_ptr<ObjectRenderer> createDefaultRenderer(std::weak_ptr<View::MapDocument> document);
            static std::unique_ptr<ObjectRenderer> createSelectionRenderer(std::weak_ptr<View::MapDocument> document);
            static std::unique_ptr<ObjectRenderer> createLockRenderer(std::weak_ptr<View::MapDocument> document);
            void clear();
        public: // color config
            void overrideSelectionColors(const Color& color, float mix);
            void restoreSelectionColors();
        public: // rendering
            void render(RenderContext& renderContext, RenderBatch& renderBatch);
        private:
            void commitPendingChanges();
            void setupGL(RenderBatch& renderBatch);
            void renderDefaultOpaque(RenderContext& renderContext, RenderBatch& renderBatch);
            void renderDefaultTransparent(RenderContext& renderContext, RenderBatch& renderBatch);
            void renderSelectionOpaque(RenderContext& renderContext, RenderBatch& renderBatch);
            void renderSelectionTransparent(RenderContext& renderContext, RenderBatch& renderBatch);
            void renderLockedOpaque(RenderContext& renderContext, RenderBatch& renderBatch);
            void renderLockedTransparent(RenderContext& renderContext, RenderBatch& renderBatch);
            void renderEntityLinks(RenderContext& renderContext, RenderBatch& renderBatch);

            void setupRenderers();
            void setupDefaultRenderer(ObjectRenderer& renderer);
            void setupSelectionRenderer(ObjectRenderer& renderer);
            void setupLockedRenderer(ObjectRenderer& renderer);
            void setupEntityLinkRenderer();

            typedef enum {
                Renderer_Default            = 1,
                Renderer_Selection          = 2,
                Renderer_Locked             = 4,
                Renderer_Default_Selection  = Renderer_Default | Renderer_Selection,
                Renderer_Default_Locked     = Renderer_Default | Renderer_Locked,
                Renderer_All                = Renderer_Default | Renderer_Selection | Renderer_Locked
            } Renderer;

            class CollectRenderableNodes;

            /**
             * This moves nodes between default / selection / locked renderers as needed,
             * but doesn't otherwise invalidate them.
             * (in particular, brushes are not updated unless they move between renderers.)
             * If brushes are modified, you need to call invalidateRenderers() or invalidateObjectsInRenderers()
             */
            void updateRenderers(Renderer renderers);
            void invalidateRenderers(Renderer renderers);
            void invalidateBrushesInRenderers(Renderer renderers, const std::vector<Model::Brush*>& brushes);
            void invalidateEntityLinkRenderer();
            void reloadEntityModels();
        private: // notification
            void bindObservers();
            void unbindObservers();

            void documentWasCleared(View::MapDocument* document);
            void documentWasNewedOrLoaded(View::MapDocument* document);

            void nodesWereAdded(const std::vector<Model::Node*>& nodes);
            void nodesWereRemoved(const std::vector<Model::Node*>& nodes);
            void nodesDidChange(const std::vector<Model::Node*>& nodes);

            void nodeVisibilityDidChange(const std::vector<Model::Node*>& nodes);
            void nodeLockingDidChange(const std::vector<Model::Node*>& nodes);

            void groupWasOpened(Model::Group* group);
            void groupWasClosed(Model::Group* group);

            void brushFacesDidChange(const std::vector<Model::BrushFace*>& faces);

            void selectionDidChange(const View::Selection& selection);

            void textureCollectionsWillChange();
            void entityDefinitionsDidChange();
            void modsDidChange();

            void editorContextDidChange();
            void mapViewConfigDidChange();

            void preferenceDidChange(const IO::Path& path);
        };
    }
}

#endif /* defined(TrenchBroom_MapRenderer) */
