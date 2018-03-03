﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace EditorResources.Functionality
{
    public static class EngineEvents
    {

        public static EventHandler<VariableMappingEventArgs> engineOnSendVariableMapping;

        public static void EngineOnSendVariableMappingEventArgs(VariableMappingEventArgs args)
        {
            engineOnSendVariableMapping?.Invoke(null, args);
        }

    }
    public class VariableMappingEventArgs : EventArgs
    {
    }

    public class AnimationVariableMappingEventArgs : VariableMappingEventArgs
    {
        public string AnimationVariable { get; set; }
    }
}
