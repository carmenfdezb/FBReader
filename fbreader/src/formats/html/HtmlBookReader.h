/*
 * FBReader -- electronic book reader
 * Copyright (C) 2004-2007 Nikolay Pultsin <geometer@mawhrin.net>
 * Copyright (C) 2005 Mikhail Sobolev <mss@mawhrin.net>
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

#ifndef __HTMLBOOKREADER_H__
#define __HTMLBOOKREADER_H__

#include <stack>

#include <shared_ptr.h>

#include "HtmlReader.h"
#include "../../bookmodel/BookReader.h"

class BookModel;
class PlainTextFormat;

class HtmlTagAction;

class HtmlBookReader : public HtmlReader {

public:
	HtmlBookReader(const std::string &baseDirectoryPath, BookModel &model, const PlainTextFormat &format, const std::string &encoding);
	~HtmlBookReader();
	void setFileName(const std::string fileName);

protected:
	virtual shared_ptr<HtmlTagAction> createAction(const std::string &tag);
	void setBuildTableOfContent(bool build);

protected:
	void startDocumentHandler();
	void endDocumentHandler();

private:
	bool tagHandler(const HtmlTag &tag);

private:
	bool characterDataHandler(const char *text, int len, bool convert);
	void addConvertedDataToBuffer(const char *text, int len, bool convert);

protected:
	BookReader myBookReader;
	std::string myBaseDirPath;

private:
	const PlainTextFormat &myFormat;
	int myIgnoreDataCounter;
	bool myIsPreformatted;
	bool myDontBreakParagraph;

	bool myIsStarted;
	bool myBuildTableOfContent;
	bool myIgnoreTitles;
	std::stack<int> myListNumStack;

	int mySpaceCounter;
	int myBreakCounter;
	std::string myConverterBuffer;

	std::map<std::string,shared_ptr<HtmlTagAction> > myActionMap;
	std::vector<TextKind> myKindList;

	std::string myFileName;

	friend class HtmlTagAction;
	friend class HtmlControlTagAction;
	friend class HtmlHeaderTagAction;
	friend class HtmlIgnoreTagAction;
	friend class HtmlHrefTagAction;
	friend class HtmlImageTagAction;
	friend class HtmlBreakTagAction;
	friend class HtmlPreTagAction;
	friend class HtmlListTagAction;
	friend class HtmlListItemTagAction;
	friend class HtmlTableTagAction;
};

#endif /* __HTMLBOOKREADER_H__ */
