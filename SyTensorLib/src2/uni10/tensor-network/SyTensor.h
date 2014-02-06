/**
 * @file SyTensor.h
 * @author Yun-Da Hsieh
 * @date 28 Aug 2013
 * @brief This is the header file for the class of symmetry tensor "SyTensor_t".
 *
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 */
#ifndef SYTENSOR_H
#define SYTENSOR_H

#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <cmath>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <assert.h>
#include <stdint.h>
#define DOUBLE	double

#include <uni10/data-structure/uni10_struct.h>
#include <uni10/data-structure/Bond.h>
#include <uni10/data-structure/Block.h>
#include <uni10/tensor-network/Matrix.h>

std::vector<_Swap> _recSwap(int* ord, int n, int* ordF);
std::vector<_Swap> _recSwap(int* _ord, int n);	//Given the reshape order out to in. 
/**
 * @brief Class of the symmetry tensor.
 */
class SyTensor_t{
	public:
    /**
     * @brief
     * How frequent it is used: *
     * @see File demo/SyTensor_basic.cpp
     */
		SyTensor_t();

    /**
     * @brief To read in a binary file of a tensor which is written out by member function @c save().\n
     * How frequent it is used: * * *
     * @param fname The file name of the tensor being loaded, which is of type STL @c std::string.
     * @see File demo/SyTensor_basic.cpp
     */
		SyTensor_t(const std::string& fname);

    /**
     * @brief To construct a tensor from a given bond array.\n
     * How frequent it is used: * * *
     * @param _bonds an STL vector of object @c Bond_t.
     * @param _name The given name of a tensor, STL std::string.
     * @see File demo/SyTensor_basic.cpp
     * @note The number of bonds must be larger than one, that is, the library does not support rank 0 tensor.
     * @warning <tt>assert( _bonds.size() > 0 )</tt>
     */
		SyTensor_t(std::vector<Bond_t>& _bonds, const std::string& _name = "");

    /**
     * @brief To construct a tensor from a given bond array and a given label array.\n
     * How frequent it is used: * *
     * @param _bonds An STL vector of object @c Bond_t.
     * @param _labels An STL interger vector, describing the labels of bonds.
     * @param _name The given name of a tensor, STL std::string.
     * @see File demo/SyTensor_basic.cpp
     * @note The number of bonds must be larger than one, that is, the library does not support rank 0 tensor.
     * @note Each label is 1-1 corresponding to each bond in the order of array.
     * @warning <tt>assert( _bonds.size() > 0 )</tt>
     * @warning <tt>assert( _bonds.size() == _labels.size() )</tt>
     */
		SyTensor_t(std::vector<Bond_t>& _bonds, std::vector<int>& labels, const std::string& _name = "");

    /**
     * @brief To construct a tensor from a given bond array and a given label array.\n
     * How frequent it is used: * *
     * @param _bonds An STL vector of object @c Bond_t.
     * @param _labels An integer array, describing the labels of bonds.
     * @param _name The given name of a tensor, STL std::string.
     * @see File demo/SyTensor_basic.cpp
     * @note The number of bonds must be larger than one, that is, the library does not support rank 0 tensor.
     * @note Each label is 1-1 corresponding to each bond in the order of array.
     * @warning <tt>assert( _bonds.size() > 0 )</tt>
     * @warning <tt>assert( _bonds.size() == _labels.size() )</tt>
     */
		SyTensor_t(std::vector<Bond_t>& _bonds, int* labels, const std::string& _name = "");

    /**
     * @brief A deep copy constructor.\n
     * How frequent it is used: * * *
     * @see File demo/SyTensor_basic.cpp
     */
		SyTensor_t(const SyTensor_t& SyT);

    /**
     * @brief A deep copy assignment.\n
     * How frequent it is used: * *
     * @see File demo/SyTensor_basic.cpp
     */
		SyTensor_t& operator=(const SyTensor_t& SyT);

		~SyTensor_t();

    /**
     * @brief Add labels to the Tensor.\n
     * How frequent it is used: * * *
     * @param newLabels An STL interger vector, describing the labels of bonds.
     * @see File demo/SyTensor_basic.cpp
     * @note Each added label is 1-1 corresponding to each bond in the order of array.
     * @warning <tt>assert( _bonds.size() == _labels.size() )</tt>
     */
		void addLabel(std::vector<int>& newLabels);

    /**
     * @brief Add labels to the Tensor.\n
     * How frequent it is used: * * *
     * @param newLabels An interger array, describing the labels of bonds.
     * @see File demo/SyTensor_basic.cpp
     * @note Each added label is 1-1 corresponding to each bond in the order of array.
     * @warning <tt>assert( _bonds.size() == _labels.size() )</tt>
     */
		void addLabel(int* newLabels);
		std::vector<int> getLabel()const;

    /**
     * @brief Add non-blocked elements to the tensor.\n
     * How frequent it is used: * * *
     * @param rawElem An array of element type of size equal to @p elemNum.
     * @see File demo/SyTensor_basic.cpp
     * @note The alignment of the given tensor elements should follow the order of the bonds.
     */
		void addRawElem(double* rawElem);

    /**
     * @brief Get the value of correspoinding array of indices.\n
     * How frequent it is used: *
     * @para idxs An STL vector of interger array, describing the indices.
     * @see File demo/SyTensor_basic.cpp
     */
		double at(std::vector<int>idxs)const;

    /**
     * @brief Get an array of quantum numbers of the blocks.\n
     * How frequent it is used: * *
     * @return An STL vector of type @c Qnum.
     * @see File demo/SyTensor_basic.cpp
     */
    	std::vector<Qnum> qnums();

    /**
     * @brief Write the tensor to an output file of filename @p fname.\n
     * How frequent it is used: *
     * @para fname A STL std::string, describing the filename of the output file.
     * @see File demo/SyTensor_basic.cpp
     */
		void save(const std::string& fname);

    /**
     * @brief Reshape the element of the tensor, that is, change the order of bonds to the order of @p newLabels and also change the element alignment to the corresponding order.\n
     * How frequent it is used: * * *
     * @param newLabels An STL interger vector, describing the labels of bonds after reshape.
     * @param rowBondNum An interger, describing the number of row bonds .
     * @see File demo/SyTensor_tool.cpp
     * @note Reshape may cause change of the order of bonds, quantum numbers of blocks of the tensor and the alignment of tensor elements.
     * @warning The only difference between @p newLabels and the original @p labels is the order of the array elements.
     */
		void reshape(std::vector<int>& newLabels, int rowBondNum);

    /**
     * @brief Reshape the element of the tensor, that is, change the order of bonds to the order of @p newLabels and also change the element alignment to the corresponding order.\n
     * How frequent it is used: * * *
     * @param newLabels An interger array, describing the labels of bonds after reshape.
     * @param rowBondNum An interger, describing the number of row bonds .
     * @see File demo/SyTensor_tool.cpp
     * @note Reshape may cause change of the order of bonds, quantum numbers of blocks of the tensor and the alignment of tensor elements.
     * @warning The only difference between @p newLabels and the original @p labels is the order of the array elements.
     */
		void reshape(int* newLabels, int rowBondNum);

    /**
     * @brief Transpose the tensor.\n
     * How frequent it is used: * * *
     * @see File demo/SyTensor_tool.cpp
     */
		void transpose();

    /**
     * @brief Randomly give a value(0 ~ 1.0) to each element.\n
     * How frequent it is used: *
     * @see File demo/SyTensor_tool.cpp
     */
		void randomize();

		void setName(const std::string& _name);
		std::string getName();
		int64_t getElemNum()const;
		int getBondNum()const;
		int getRBondNum()const;

		void check();
		friend std::ostream& operator<< (std::ostream& os, SyTensor_t& SyT);
		friend SyTensor_t operator* (SyTensor_t& Ta, SyTensor_t& Tb);
		SyTensor_t& operator*= (SyTensor_t& Tb);
		friend SyTensor_t operator+ (const SyTensor_t& Ta, const SyTensor_t& Tb);
		void operator+= (const SyTensor_t& Tb);
		friend SyTensor_t operator* (const SyTensor_t& Ta, double a);
		friend SyTensor_t operator* (double a, const SyTensor_t& Ta){return Ta * a;};
		void operator*= (double a);
		Matrix_t getBlock(Qnum qnum, bool diag = false);
		void putBlock(const Qnum& qnum, Matrix_t& mat);
		std::map<Qnum, Matrix_t> getBlocks();
		Matrix_t printRawElem(bool flag = true);
		friend class Node_t;
		friend class Network_t;
		void orthoRand();
		void orthoRand(const Qnum& qnum);
		void eye();
		void eye(const Qnum& qnum);
		void bzero(const Qnum& qnum);
		void bzero();
		std::vector<_Swap> exSwap(const SyTensor_t& Tb)const;
		bool similar(const SyTensor_t& Tb)const;
		void addGate(std::vector<_Swap> swaps);
		bool elemCmp(const SyTensor_t& SyT)const;
		double trace()const;
		double trace(const SyTensor_t&)const;
		void combineIndex(const std::vector<int>& combined_labels);
		//SyTensor_t partialTrace(const std::vector<int>& labelA, const std::vector<int>& labelB)const;
		SyTensor_t& partialTrace(int la, int lb);
	private:
		std::string name;
		DOUBLE *elem;		//Array of elements
		int status;	//Check initialization, 1 initialized, 3 initialized with label, 5 initialized with elements
		std::vector<Bond_t> bonds;
		std::map<Qnum, Block_t> blocks;
		std::vector<int>labels;
		void packMeta();
		int RBondNum;	//Row bond number
		int RQdim;
		int CQdim;
		int64_t elemNum;
		std::map<int, Block_t*> RQidx2Blk;	//Qidx to the Block
		std::map<int, int> QidxEnc;
		std::map<int, int> RQidx2Off;	//the row offset starts from the block origin of a qnum
		std::map<int, int> CQidx2Off;	//the col offset starts from the block origin of a qnum
		std::map<int, int> RQidx2Dim;
		std::map<int, int> CQidx2Dim;
		static int COUNTER;
		static int64_t ELEMNUM;
		static int64_t MAXELEMNUM;
		static int64_t MAXELEMTEN;	//Max number of element of a tensor
		//Private Functions
		void grouping();
		void initSyT();
		static const int INIT = 1;		      /**< A flag for initialization */
		static const int HAVELABEL = 2;		/**< A flag for having labels added */
		static const int HAVEELEM = 4;		  /**< A flag for having element assigned */
};
#endif /* SYTENSOR_H */
