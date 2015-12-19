/*	This program takes a fileName and renames the whole batch
**	format of renaming: title episode year quality extras extension.
**/

#include <bits/stdc++.h>

using namespace std;

string fileName, fileExt, newName, title, preEp, remWord, insPhrase, episode, quality, year, extension, extras;
string buffer, bufferExt;
vector <string> words, titleWords;
map <string, bool> exts, titleMap, remWordMap;
int len;

void explode(vector <string> &space, const string &fileName) {
	string valid, chk;
	for(int i = 0; i < len; i++) {
		if(isalnum(fileName[i]))
			valid += fileName[i];
		else {
			chk = valid;
			transform(chk.begin(), chk.end(), chk.begin(), ::tolower);
			if(valid.length() && !remWordMap[chk]) //skip if the word belongs to remove phrase
				space.push_back(valid);
			valid = "";
		}
	}
	if(valid.length())
		space.push_back(valid);
}

void trim(string &str) {
	int i = 0, strl = str.length();
	while(i < strl && str[i] == ' ') i++;
	str.erase(str.begin(), str.begin() + i);
	i = str.length() - 1;
	while(i >= 0 && str[i] == ' ') i--;
	str.erase(str.begin() + i + 1, str.end());
}

void removeEpPrefix () {
	int preEpLen = preEp.length();
	for(int i = 0; i < words.size(); i++)
		if(words[i].length() > preEpLen && words[i].substr(0, preEpLen) == preEp) {
			words[i].erase(words[i].begin(), words[i].begin() + preEpLen);
			break;
		}
}

void initExt() {
	exts["mp4"] = true;
	exts["flv"] = true;
	exts["avi"] = true;
	exts["mkv"] = true;
	exts["wmv"] = true;
}

void initTitle() {
	for(int i = 0; i < titleWords.size(); i++) {
		transform(titleWords[i].begin(), titleWords[i].end(), titleWords[i].begin(), ::tolower);
		titleMap[titleWords[i]] = true;
	}
}

string getEpisode() {
	int slen, epNo;
	bool valid;
	string ret;
	for(int i = 0; i < words.size(); i++) {
		slen = words[i].length();
		epNo = 0;
		valid = true;
		for(int j = 0; j < slen; j++) {
			if(!isdigit(words[i][j])) {
				valid = false;
				break;
			}
			epNo = epNo * 10 + words[i][j] - '0';
		}
		if(valid && epNo < 1900) {
			ret = " " + words[i];
			words.erase(words.begin() + i);
			return ret;
		}
	}
	return "";
}

string getQuality() {
	int slen;
	string ret;
	bool valid;
	for(int i = 0; i < words.size(); i++) {
		slen = words[i].length();
		valid = true;
		for(int j = 0; j < slen - 1; j++)
			if(!isdigit(words[i][j])) {
				valid = false;
				break;
			}
		if(valid && tolower(words[i][slen - 1]) == 'p') {
			ret = " @" + words[i].substr(0, slen - 1) + 'p';
			words.erase(words.begin() + i);
			return ret;
		}
	}
	return "";
}

string getYear() {
	int slen, year;
	bool valid;
	string ret;
	for(int i = 0; i < words.size(); i++) {
		slen = words[i].length();
		year = 0;
		valid = true;
		for(int j = 0; j < slen; j++) {
			if(!isdigit(words[i][j])) {
				valid = false;
				break;
			}
			year = year * 10 + words[i][j] - '0';
		}
		if(valid && year > 1900 && words[i].length() == 4) {
			ret = " (" + words[i] + ")";
			words.erase(words.begin() + i);
			return ret;
		}
	}
	return "";
}

string getExtension() {
	string ret;
	for(int i = 0; i < words.size(); i++) {
		if(exts[words[i]]) {
			transform(words[i].begin(), words[i].end(), words[i].begin(), ::tolower);
			ret = "." + words[i];
			words.erase(words.begin() + i);
			return ret;
		}
	}
	return "";
}

void removeExtraNos() {
	int wordLen;
	bool onlyDigit;
	for(int i = 0; i < words.size(); i++) {
		onlyDigit = true;
		wordLen = words[i].length();
		for(int j = 0; j < wordLen; j++)
			if(!isdigit(words[i][j])) {
				onlyDigit = false;
				break;
			}
		if(onlyDigit) {
			words.erase(words.begin() + i);
			i--;
		}
	}
}

string getExtras() {
	string ret = " - ", lowerTitleTest;
	for(int i = 0; i < words.size(); i++) {
		lowerTitleTest = words[i];
		transform(lowerTitleTest.begin(), lowerTitleTest.end(), lowerTitleTest.begin(), ::tolower);
		if(!titleMap[lowerTitleTest])
			ret += words[i] + " ";
	}
	ret.erase(ret.end() - 1);
	if(ret.length() < 3)
		return "";
	return ret;
}

void renameFile(const string &fileName) {
	len = fileName.length();
	explode(words, fileName);
	removeEpPrefix();
	episode = getEpisode();
	quality = getQuality();
	year = getYear();
	extension = getExtension();
	removeExtraNos();
	extras = getExtras() + insPhrase; //this should be done last as this extracts all the words that are left
	newName = title + episode + year + quality + extras + extension;
	if(episode == "")
		cout << fileName << " >> Parse Failed!" << endl;
	else {
		buffer = fileName;
		cout << "Rename " << buffer << "  -->>  " << newName <<endl;
		//if(rename(buffer.c_str(), newName.c_str()))
		//	cout << "Rename Fail >> " << fileName << endl;
	}
	words.clear();
}

int main () {
	initExt();
	char ch;
	locale loc;
	while((ch = getchar()) > 10) //get title
		title += ch;
	while((ch = getchar()) > 10) //get prefix before episode number
		preEp += ch;
	while((ch = getchar()) > 10) { //get phrase to remove
		if(ch == ' ') {
			remWordMap[remWord] = true;
			remWord = "";
		}
		else remWord += tolower(ch);
	}
	if(remWord.length())
		remWordMap[remWord] = true;
	while((ch = getchar()) > 10) //get phrase to insert at end [CASE SENSITIVE]
		insPhrase += ch;
	if(insPhrase.length())
		insPhrase.insert(insPhrase.begin(), ' ');
	trim(title);
	trim(preEp);
	trim(insPhrase);
	len = title.length();
	explode(titleWords, title);
	initTitle();
	ifstream fileList;
	fileList.open("filelist.txt");
	while(getline(fileList, fileName)) {
		if(fileName.length() > 3)
			fileExt = fileName.substr(fileName.length() - 3);
		if(fileExt == "mp4" || fileExt == "avi" || fileExt == "wmv" || fileExt == "mkv" || fileExt == "flv") 
			renameFile(fileName);
	}
	fileList.close();
	return 0;
}