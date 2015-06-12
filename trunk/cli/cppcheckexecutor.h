/*
 * Cppcheck - A tool for static C/C++ code analysis
 * Copyright (C) 2007-2012 Daniel Marjamäki and Cppcheck team.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once
#ifndef CPPCHECKEXECUTOR_H
#define CPPCHECKEXECUTOR_H

#include "errorlogger.h"
#include <ctime>
#include <set>
#include <string>
#include "FileDepend.h"

class CppCheck;
class Settings;

/**
 * This class works as an example of how CppCheck can be used in external
 * programs without very little knowledge of the internal parts of the
 * program itself. If you wish to use cppcheck e.g. as a part of IDE,
 * just rewrite this class for your needs and possibly use other methods
 * from CppCheck class instead the ones used here.
 */
class CppCheckExecutor : public ErrorLogger {
public:
    /**
     * Constructor
     */
    CppCheckExecutor();

    /**
     * Destructor
     */
    virtual ~CppCheckExecutor();

    /**
     * Starts the checking.
     *
     * @param argc from main()
     * @param argv from main()
     * @return EXIT_FAILURE if arguments are invalid or no input files
     *         were found.
     *         If errors are found and --error-exitcode is used,
     *         given value is returned instead of default 0.
     *         If no errors are found, 0 is returned.
     */
    int check(int argc, const char* const argv[]);

	void WriteXml();

	void readcfg();
	//from TSC 20141016 check if error's subid is in AUTOFILTER_SUBID
	bool isInAutoFilterSubID(std::string erroritem);
	//from TSC 20141016 add Intelligent filtering
	void autoFilterResult();

	//here from TSC 20140226
	 void dealretnulllog();
	 void dealretaddrlog();
	//form TSC 20140707
	 /*@brief filt the error list before written into xml file.
	  *some errors need to be modify,the "subid" and "msg" content
	  *should be more accurate.
	  *find out those errors due to functions return null.
	  */
	 //void filtResultRetNull();
	 /*@brief filt the error list before written into xml file.
	  *some errors need to skip,reduce the mount of false negative.
	  *some errors due to assign from a function which return a address.
	  */
	 //void filtResultRetAddr();

	//
    /**
     * Information about progress is directed here. This should be
     * called by the CppCheck class only.
     *
     * @param outmsg Progress message e.g. "Checking main.cpp..."
     */
    virtual void reportOut(const std::string &outmsg);

    /** xml output of errors */
    virtual void reportErr(const ErrorLogger::ErrorMessage &msg);

    void reportProgress(const std::string &filename, const char stage[], const std::size_t value);

    /**
     * Output information messages.
     */
    virtual void reportInfo(const ErrorLogger::ErrorMessage &msg);

    /**
     * Information about how many files have been checked
     *
     * @param fileindex This many files have been checked.
     * @param filecount This many files there are in total.
     * @param sizedone The sum of sizes of the files checked.
     * @param sizetotal The total sizes of the files.
     */
    static void reportStatus(std::size_t fileindex, std::size_t filecount, std::size_t sizedone, std::size_t sizetotal);

protected:

    /**
     * Helper function to print out errors. Appends a line change.
     * @param errmsg String printed to error stream
     */
    void reportErr(const std::string &errmsg);

    /**
     * @brief Parse command line args and get settings and file lists
     * from there.
     *
     * @param cppcheck cppcheck instance
     * @param argc argc from main()
     * @param argv argv from main()
     * @return false when errors are found in the input
     */
    bool parseFromArgs(CppCheck *cppcheck, int argc, const char* const argv[]);

private:

    /**
     * Pointer to current settings; set while check() is running.
     */
	std::set<std::string>  filterdir;
    const Settings* _settings;

    /**
     * Used to filter out duplicate error messages.
     */
	std::set<std::string> _errorList;
	//from TSC 20140709
	std::list<std::string> errorListresult;

    /**
     * Filename associated with size of file
     */
    std::map<std::string, std::size_t> _files;

	/**
     * add by TSC, 2014-10-13
     */
    CFileDependTable _fileDependTable;

    /**
     * Report progress time
     */
    std::time_t time1;

    /**
     * Has --errorlist been given?
     */
    bool errorlist;
};

#endif // CPPCHECKEXECUTOR_H