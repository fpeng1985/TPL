/*!
 * \file utils.h
 * \author Peng Fei
 * \brief Utility types and variables.
 */

#ifndef TPL_UTILS_H
#define TPL_UTILS_H

#include <Eigen/Sparse>
#include <Eigen/IterativeLinearSolvers>
//#include <Eigen/Cholesky>

namespace tpl {

    using VectorXd = Eigen::VectorXd;
	using MatrixXd = Eigen::MatrixXd;

    /*!
     * \typedef Eigen::SparseMatrix<double> SpMat;
     * \brief SpMat type
     */
    using SpMat = Eigen::SparseMatrix<double>;

    /*!
     * \typedef Eigen::Triplet<double> SpElem;
     * \brief SpElem type
     */
    using SpElem = Eigen::Triplet<double>;

    /*!
     * \typedef Eigen::SimplicialLLT<SpMat> LLTSolver;
     * \brief LLTSolver type
     */
    using LLTSolver = Eigen::SimplicialLLT<SpMat>;
}

#endif //TPL_UTILS_H
