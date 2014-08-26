/***************************************************************************
 *
 * Copyright 2010,2011 BMW Car IT GmbH
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ****************************************************************************/
#include "RemoveSynchronizedSurfacesCommand.h"
#include "ICommandExecutor.h"
#include "Scene.h"
#include "Log.h"

ExecutionResult RemoveSynchronizedSurfacesCommand::execute(ICommandExecutor* executor)
{
    Scene& scene = *(executor->getScene());
    ExecutionResult result = ExecutionFailed;

    LOG_DEBUG("RemoveSynchronizedSurfacesCommand", "Length to set: " << m_length);

    // First of all get all surfaces which are in the current render order.
    for (unsigned int i = 0; i < m_length; i++)
    {
        Surface* surface = scene.getSurface(m_array[i]);
        if (surface)
        {
            surface->setSynchronized(false);
            LOG_DEBUG("RemoveSynchronizedSurfacesCommand", "Unsetting synchronized Surface : " << m_array[i]);
            result = ExecutionSuccess;
        }
    }
    return result;
}

const std::string RemoveSynchronizedSurfacesCommand::getString()
{
    std::stringstream description;
    description << "RemoveSynchronizedSurfacesCommand("
                << "m_array=[";

    for (unsigned int i = 0; i < m_length; ++i)
    {
        description << m_array[i] << ",";
    }
    description << "], m_length=" << m_length
                << ")";
    return description.str();
}
