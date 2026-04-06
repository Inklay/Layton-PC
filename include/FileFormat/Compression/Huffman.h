#pragma once

#include "Utils/FileUtils.h"

class Huffman {
	static fileUtils::buffer decompress(const fileUtils::path& filePath, size_t offset = 0);

	friend fileUtils;

	class TreeNode {
	public:
		TreeNode(const fileUtils::buffer& buffer, bool isData, size_t offset);
		unsigned char m_data;
		bool m_isData;
		TreeNode* m_parent;
		std::unique_ptr<TreeNode> m_childA;
		std::unique_ptr<TreeNode> m_childB;
	};
};
