#include <string>
#include <vector>
#include<boost/filesystem.hpp>
#include<boost/random.hpp>
#include<boost/tokenizer.hpp>
#include"opencv2/opencv.hpp"
namespace fs = boost::filesystem;
using namespace std;
using namespace cv;


bool GetDirFiles(string dir, vector<string>& filepath)
{
	fs::path fullpath(dir);
	if (!fs::exists(fullpath))
	{
		return false;
	}
	fs::recursive_directory_iterator end_iter;
	for (fs::recursive_directory_iterator iter(fullpath); iter != end_iter; iter++)
	{
		try {
			if (!fs::is_directory(*iter) && (fs::extension(*iter) == ".jpg" || fs::extension(*iter) == ".JPG"
				|| fs::extension(*iter) == ".jpeg" || fs::extension(*iter) == ".bmp" || fs::extension(*iter) == ".avi"
				|| fs::extension(*iter) == ".txt"))
			{
				string name = iter->path().string();
				filepath.emplace_back(name);
			}
		}
		catch (const std::exception& ex) {
			continue;
		}
	}
	return true;
}

void ChangeCocoLabels() {
	const int NUMCLS = 80;
	static int coco_ids[] = { 1,2,3,4,5,6,7,8,9,10,11,13,14,15,16,17,18,19,20,21,22,23,24,25,27,28,31,32,33,34,35,36,37,38,39,40,41,42,43,44,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,67,70,72,73,74,75,76,77,78,79,80,81,82,84,85,86,87,88,89,90 };
	//coco 不连续标签对应的连续标签
	vector<std::pair<int,int> >couple_index;
	for (int i=0;i< NUMCLS;i++)
	{
		couple_index.emplace_back(make_pair(coco_ids[i], i));
	}
	const string label_dir = "F:/COCO17/COCO/labels-0/test2017";
	vector<string> labels_path;
	GetDirFiles(label_dir, labels_path);



	for (int i=0;i<labels_path.size();i++)
	{
			typedef boost::tokenizer<boost::char_separator<char>> CustomTokenizer;
			boost::char_separator<char> sep1("/\\");
			CustomTokenizer tok(labels_path[i], sep1);
			vector<string> vecseg_tag;
			for (CustomTokenizer::iterator beg = tok.begin(); beg != tok.end(); ++beg)
				vecseg_tag.emplace_back(*beg);
			string save_txtpath = "F:/COCO17/COCO/labels/test2017/" + vecseg_tag[vecseg_tag.size() - 1];
		ifstream ifs(labels_path[i]);
		//获取当前路径下的文件名


		ofstream ofs(save_txtpath);

		string line;
		while (getline(ifs,line))
		{
			//文件名解析
			boost::char_separator<char> sep(" ");
			CustomTokenizer tok1(line, sep);
			vector<string> seg_tag;
			for (CustomTokenizer::iterator beg = tok1.begin(); beg != tok1.end(); ++beg)
				seg_tag.emplace_back(*beg);

			//获取第一个位置的标签信息。
			int cls_id = atoi(seg_tag[0].c_str());
			for (int idx=0;idx<NUMCLS;idx++)
			{
				if (cls_id==couple_index[idx].first)
				{
					seg_tag[0] = to_string(couple_index[idx].second);
					break;
				}
			}
			//文件重新写入
			for (int i=0;i<seg_tag.size();i++)
			{
				if (i==seg_tag.size()-1)
				{
					ofs << seg_tag[i] << std::endl;
				}
				else {
				ofs << seg_tag[i] << " ";
				}
			}

		}

		ifs.close();
		ofs.close();
	}
}

void main() {
	ChangeCocoLabels();

}
