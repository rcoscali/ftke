/* ------------------------------------------------------------------
 * Copyright (C) 1998-2009 PacketVideo
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 * -------------------------------------------------------------------
 */
/*
    This DataEntryUrlAtom Class contains a table of data references which declare
    the location of the media data used within the MPEG-4 presentation.
*/


#ifndef DATAENTRYURLATOM_H_INCLUDED
#define DATAENTRYURLATOM_H_INCLUDED

#ifndef OSCL_FILE_IO_H_INCLUDED
#include "oscl_file_io.h"
#endif
#ifndef DATAENTRYATOM_H_INCLUDED
#include "dataentryatom.h"
#endif

class DataEntryUrlAtom : public DataEntryAtom
{

    public:
        DataEntryUrlAtom(MP4_FF_FILE *fp); // Stream-in ctor
        virtual ~DataEntryUrlAtom();

        // Member gets and sets
        OSCL_wString& getLocation()
        {
            return _location;
        }

    private:
        OSCL_wHeapString<OsclMemAllocator> _location;

};

#endif // DATAENTRYURLATOM_H_INCLUDED
