
using System.Collections.Generic;
using System.Linq;

using DSI4 = GenTool_CsDataServerDomAsr4.Iface;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleRte : IModule
  {
    public string Name
    {
      get { return "Rte"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/Rte"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string genDataDir = model.Directory.GenData;

        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Rte.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Rte.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Rte_Cbk.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Rte_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Rte_Compiler_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Rte_DataHandleType.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Rte_Hook.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Rte_VttHook.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Rte_Main.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Rte_MemMap.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Rte_Type.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Rte_UserTypes.h"), Name);

        string componentsDir = System.IO.Path.Combine(genDataDir, "Components");

        if (System.IO.Directory.Exists(componentsDir))
        {
          model.Project.AddIncludeDirectory(componentsDir);

          foreach (string file in System.IO.Directory.EnumerateFiles(componentsDir, "*.h", System.IO.SearchOption.TopDirectoryOnly))
          {
            if (!System.IO.Path.GetFileName(file).StartsWith("_"))
            {
              model.Project.AddBswFile(file, "RteComponents");
            }
          }
        }


        DSI4.IEcucModuleConfigurationValues[] moduleValues = ecuc.FindModuleConfigurationsByDefinitionPath("/MICROSAR/VTT/VTTOs");

        // Variante 1: Mit der Hand am Arm
        List<string> applicationNames = new List<string>();
        foreach(DSI4.IEcucModuleConfigurationValues mV in moduleValues)
        {
          foreach (var ecuccv in mV.ContainerList.OfType<DSI4.IEcucContainerValue>())
          {
            if (ecuccv.DefinitionRef.Value == "/MICROSAR/VTT/VTTOs/OsApplication")
            {
              string name = ecuccv.ShortName;
              applicationNames.Add(name);
            }
          }
        }

        // Variante 2: Funky mit Linq ;-) F�r diese Variante bitte Z. 61-73 auskommentieren/l�schen.
        // var applicationNames =
        //  from mcv in moduleValues
        //  from ecuccv in mcv.ContainerList.OfType<DSI4.IEcucContainerValue>()
        //  where ecuccv.DefinitionRef.Value == "/MICROSAR/VTT/VTTOs/OsApplication"
        //  select ecuccv.ShortName;

        foreach (string appName in applicationNames)
        {
          string rteFile = System.IO.Path.Combine(genDataDir, "Rte_" + appName + ".c");
          if (System.IO.File.Exists(rteFile))
          {
            model.Project.AddBswFile(rteFile, Name);
          }
        }

        // RTE hooks generated by VTT
        // model.Project.AddBswFile(System.IO.Path.Combine(model.Directory.Source, "Rte_Hook.c"), Name);
        // model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Vtt_Rte.h"), Name);
        // model.CANoeEmu.AddStateChangeFunction("VttRte_OnStateChange", "Vtt_Rte.h");
      }
    }
  }
}

