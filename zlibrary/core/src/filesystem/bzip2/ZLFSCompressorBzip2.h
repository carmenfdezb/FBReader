/*
 * Copyright (C) 2004-2012 Geometer Plus <contact@geometerplus.com>
 * Contributed by Serge Osnach <serge.osnach@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#ifndef __ZLFSCOMPRESSORBZIP2_H__
#define __ZLFSCOMPRESSORBZIP2_H__

#include "../ZLFSPlugin.h"

class ZLFSCompressorBzip2 : public ZLFSCompressor {

public:
	ZLFSCompressorBzip2();
	virtual ZLFile::ArchiveType prepareFile(ZLFile &file, std::string & nameWithoutExt);
	virtual const std::string signature() const;
	virtual shared_ptr<ZLInputStream> envelope(ZLFile::ArchiveType &type, shared_ptr<ZLInputStream> base);
	virtual ~ZLFSCompressorBzip2();
};

#endif /* __ZLFSCOMPRESSORBZIP2_H__ */