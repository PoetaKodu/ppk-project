#pragma once

#include "Record.h"

template <std::size_t TMaxAttributes>
class DecisionTree
{
public:
	constexpr static std::size_t MaxAttributes = TMaxAttributes;
	using StructureType = RecordStructure<MaxAttributes>;
	using RecordType 	= AttributeArray<MaxAttributes>;

	StructureType recordStructure;

	void loadRecordStructure(std::string const& str_)
	{
		return this->loadRecordStructure(str_.data(), str_.data() + str_.size());
	}

	void loadRecordStructure(char const* beg_, char const* end_);
};

#include "DecisionTree.inl"

