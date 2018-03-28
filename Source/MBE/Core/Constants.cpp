#include <MBE/Core/Constants.h>

using namespace std;
using namespace mbe;

#pragma region Constructor and Destructor

Constants::Constants()
{
	// Initialise the pointers with null
	screenWidth = nullptr;
	screenHeight = nullptr;

	this->hasLoadedSuccessfully = false;
	this->hasLoadedSuccessfully = Load("Resources/constants.txt");
}


Constants::~Constants()
{
	delete screenWidth;
	delete screenHeight;

	screenWidth = nullptr;
	screenHeight = nullptr;
}

#pragma endregion

#pragma region Public Methods

// Opens the initialisation file and initialises all the constants
// Checks every line for every constant
// This is to ensure that no constant is defined more than once
// Returns false if not every constant could be initialised
bool Constants::Load(string filePath)
{
	// If the file could not be read
	if (!ReadFile(filePath))
		return false;

	if (!AddContent<unsigned short int>(&screenWidth, "screen width", detail::RegularExpression::positveInteger))
		return false;
	if (!AddContent<unsigned short int>(&screenHeight, "screen height", detail::RegularExpression::positveInteger))
		return false;
	if (!AddContent<float>(&testTimeMultiplicator, "test time multiplicator", detail::RegularExpression::positiveDecimal))
		return false;

	return true;
}


#pragma endregion

#pragma region Private Methods

// Reads in the file
// Only reads the lines matching the regular expression for specifying a constant
// Returns true if the file has been read in successfully
bool Constants::ReadFile(string filePath)
{
	// Creating the regular expression
	try
	{
		// Regular expressions are compliled during runtime
		// If the string parameter contains any errors, the regex constructor will thow an excpetion
		regex constantDefinitionRegularExpression(
			string("[a-z][a-z\\s]*")										// (One lower case letter) ([optional] any combination of lower case letters and white spaces)
			+ string(":\\s")												// A colon followed by a white space
			+ string("((\\+|-)?[[:digit:]]+)((\\.|,)(([[:digit:]]+)?))?")	// Any negative or positive decimal [includes integers]
		);

		ifstream file(filePath);

		if (file.is_open())
		{
			string line;
			while (getline(file, line))
			{
				// Select only the lines that match the regular expression
				if (regex_match(line, constantDefinitionRegularExpression))
					lines.push_back(line);
			}
			file.close();
			cerr << "File opened successfully!" << endl << endl;
			return true;
		}
		else
			// The function returns false anyway
			cerr << "Error: Unable to open file!" << endl << endl;
	}
	catch (const std::exception & e)
	{
		cerr << endl << "An error occured when creating the standard expression to match general constant definitions:" << endl;
		cerr << e.what() << endl << endl;
	}
	return false;
}

// Searches for a given content in a given line of text
// The content is expected to be in the form:
// [constant name]: [content (usually a number)]
// Extracts the content after the constant name and checks it againt a regular expression
// If the content has not been found or an erorr occured --> return the contentDoesNotMatch parameter
string Constants::FindContent(string searchContent, string lineToSearch, detail::RegularExpression expressionToMatch)
{
	auto pos = lineToSearch.find(searchContent);
	if (pos != string::npos)
	{
		string content;
		content = lineToSearch.substr(pos + searchContent.length() + 2); // + 2 --> the ":" and " "
		try
		{
			// This may be result in an error if the regular expression is invalid
			if (regex_match(content, regex(GetRegularExpressionString(expressionToMatch))))
				return content;
		}
		catch (const std::exception&)
		{
			cerr << endl << "Regular expression compilation error: Invalid regular expression!" << endl;
		}
		// If the content has not been found or an erorr occured --> return the contentDoesNotMatch parameter
	}
	return string(detail::contentDoesNotMatch);
}

// Returns false if the desired variable could not be initialised
// The double pointer is needed so that the content of the (pointer) constantToDefine is changed
// If only a single pointer was used, the argument passed would still be null even if the content of the parameter would have been changed
template <typename TData> bool Constants::AddContent(TData** constantToDefine, string constantName, detail::RegularExpression expressionToMatch)
{
	// for each line in the file
	for (auto line : lines)
	{
		// search for the constant name and extract the data if found
		string content = Constants::FindContent(constantName, line, expressionToMatch);
		// if the content has been found
		if (content != detail::contentDoesNotMatch)
		{
			// And the constant has not been initialised before
			if (*constantToDefine == nullptr)
			{
				// Create the constant (allocate memory)
				*constantToDefine = new TData();

				// Asign the matching value to it
				switch (expressionToMatch)
				{
				case detail::RegularExpression::integer:
				case detail::RegularExpression::positveInteger:
					**constantToDefine = stoi(content);
					break;
				case detail::RegularExpression::decimal:
				case detail::RegularExpression::positiveDecimal:
					**constantToDefine = stod(content);
					break;
				default:
					break;
				}
			}
			// If the constant has been defined more than once
			else
			{
				cerr << endl << "Constant initialisation error: " << "'" << constantName << "' " << "has been defined more than once (first value taken [" << **constantToDefine << "])";
			}
		}
	}

	// If the constant could not be initialised (it is still undefined / has not been created yet)
	if (*constantToDefine == nullptr)
	{
		cerr << endl << "Constant initialisation error: " << "'" << constantName << "' " << "could not be initialised (no matching value found)";
		return false;
	}
	// If the constant has been initialised --> return true
	return true;
}

#pragma endregion
