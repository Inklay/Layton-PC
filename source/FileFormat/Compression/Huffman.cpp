#include "FileFormat/Compression/Huffman.h"

fileUtils::buffer Huffman::decompress(const fileUtils::path& filePath, size_t offset) {
	const fileUtils::buffer buffer = fileUtils::readBin(filePath, offset);
	const unsigned int blockSize = buffer.at(0) == 0x24 ? 4 : 8;
	const size_t decompressedSize = 0 | (buffer.at(3) << 16) | (buffer.at(2) << 8) | buffer.at(1);
	fileUtils::buffer decompressedBuffer;
	decompressedBuffer.reserve(decompressedSize);
	const size_t treeSize = (buffer.at(4) + 1) * 2;
	size_t bufferOffset = 4 + treeSize;
	size_t decompressed = 0;
	size_t bitsLeft = 0;
	TreeNode root(buffer, false, 5);
	TreeNode* currentNode = &root;
	uint32_t data;
	int cachedByte = -1;

	while (decompressed < decompressedSize) {
		while (!currentNode->m_isData) {
			if (bitsLeft == 0) {
				data = (buffer.at(bufferOffset + 3) << 24) | (buffer.at(bufferOffset + 2) << 16) | (buffer.at(bufferOffset + 1) << 8) | buffer.at(bufferOffset);
				bufferOffset += 4;
				bitsLeft = 32;
			}
			bitsLeft--;
			currentNode = data & (1 << bitsLeft) ? currentNode->m_childB.get() : currentNode->m_childA.get();
		}
		
		if (blockSize == 8) {
			decompressedBuffer.emplace_back(currentNode->m_data);
			decompressed++;
		} else {
			if (cachedByte == -1) {
				cachedByte = currentNode->m_data << 4;
			} else {
				cachedByte |= currentNode->m_data;
				decompressedBuffer.emplace_back(cachedByte);
				decompressed++;
				cachedByte = -1;
			}
		}

		currentNode = &root;
	}

	return decompressedBuffer;
}

Huffman::TreeNode::TreeNode(const fileUtils::buffer& buffer, bool isData, size_t offset) {
	m_data = buffer.at(offset);
	m_isData = isData;
	m_parent = nullptr;

	if (!m_isData) {
		unsigned int dataOffset = m_data & 0x3F;
		unsigned int childAIsData = (m_data & 0x80) > 0;
		unsigned int childBIsData = (m_data & 0x40) > 0;
		unsigned int childOffset = (offset ^ (offset & 1)) + dataOffset * 2 + 2;

		m_childA = std::make_unique<TreeNode>(buffer, childAIsData, childOffset);
		m_childB = std::make_unique<TreeNode>(buffer, childBIsData, childOffset + 1);

		m_childA->m_parent = this;
		m_childB->m_parent = this;
	}
}
