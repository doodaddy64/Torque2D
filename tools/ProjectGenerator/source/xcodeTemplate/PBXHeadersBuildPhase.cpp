//-----------------------------------------------------------------------------
// 3 Step Studio
// Copyright GarageGames, LLC 2012
//-----------------------------------------------------------------------------

#include "xcodeTemplate/PBXHeadersBuildPhase.h"
#include "xcodeTemplate/PBXBuildFile.h"

IMPLEMENT_GLOBAL_LIST(PBXHeadersBuildPhase)

PBXHeadersBuildPhase::PBXHeadersBuildPhase()
{
    SetName("Headers");

    // Add ourselves to the global list
    ADD_TO_GLOBAL_LIST()
}

PBXHeadersBuildPhase::~PBXHeadersBuildPhase()
{
}

IMPLEMENT_WRITE_GLOBAL_LIST(PBXHeadersBuildPhase)
{
    stream << endl << "/* Begin PBXHeadersBuildPhase section */" << endl;
    U32 size = m_List.size();
    for(U32 i=0; i<size; ++i)
    {
        PBXHeadersBuildPhase* phase = m_List[i];

        stream << "\t\t";
        phase->GetUUID().Write(stream);
        if(!phase->GetName().empty())
        {
            stream << " /* " << phase->GetName() << " */";
        }
        stream << " = {" << endl;

        stream << "\t\t\tisa = " << phase->GetISA() << ";" << endl;
        stream << "\t\t\tbuildActionMask = " << phase->m_BuildActionMask << ";" << endl;

        stream << "\t\t\tfiles = (" << endl;
        for(U32 j=0; j<phase->m_Files.size(); ++j)
        {
            stream << "\t\t\t\t";
            phase->m_Files[j]->GetUUID().Write(stream);
            if(!phase->m_Files[j]->GetName().empty())
            {
                stream << " /* " << phase->m_Files[j]->GetName() << " in " << phase->GetName() << " */";
            }
            stream << "," << endl;
        }
        stream << "\t\t\t);" << endl;

        stream << "\t\t\trunOnlyForDeploymentPostprocessing = " << phase->m_RunOnlyForDeploymentPostprocessing << ";" << endl;

        stream << "\t\t};" << endl;
    }

    stream << "/* End PBXHeadersBuildPhase section */" << endl;

    return true;
}
