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

#ifndef TrenchBroom_CurrentGroupCommand
#define TrenchBroom_CurrentGroupCommand

#include "Macros.h"
#include "Model/Model_Forward.h"
#include "View/UndoableCommand.h"
#include "View/View_Forward.h"

#include <memory>

namespace TrenchBroom {
    namespace View {
        class CurrentGroupCommand : public UndoableCommand {
        public:
            static const CommandType Type;
        private:
            Model::Group* m_group;
        public:
            static std::unique_ptr<CurrentGroupCommand> push(Model::Group* group);
            static std::unique_ptr<CurrentGroupCommand> pop();

            CurrentGroupCommand(Model::Group* group);
        private:
            std::unique_ptr<CommandResult> doPerformDo(MapDocumentCommandFacade* document) override;
            std::unique_ptr<CommandResult> doPerformUndo(MapDocumentCommandFacade* document) override;

            bool doCollateWith(UndoableCommand* command) override;
            bool doIsRepeatable(MapDocumentCommandFacade* document) const override;

            deleteCopyAndMove(CurrentGroupCommand)
        };
    }
}

#endif /* defined(TrenchBroom_CurrentGroupCommand) */
