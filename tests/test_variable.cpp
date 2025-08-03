#include <gtest/gtest.h>
#include "../models/terms/Variable.h"
#include "../exceptions/Exceptions.h"

class VariableTest : public ::testing::Test {
protected:
    void SetUp() override {
        var_x = std::make_unique<Variable>("x");
        var_y = std::make_unique<Variable>("y");
    }

    std::unique_ptr<Variable> var_x;
    std::unique_ptr<Variable> var_y;
};

TEST_F(VariableTest, Constructor) {
    Variable var("test");
    EXPECT_EQ(var.name, "test");
}

TEST_F(VariableTest, Clone) {
    auto cloned = var_x->clone();
    Variable* cloned_var = dynamic_cast<Variable*>(cloned.get());
    ASSERT_NE(cloned_var, nullptr);
    EXPECT_EQ(cloned_var->name, "x");
    EXPECT_NE(cloned_var, var_x.get());
}

TEST_F(VariableTest, IsNormal) {
    EXPECT_TRUE(var_x->is_normal());
    EXPECT_TRUE(var_y->is_normal());
}

TEST_F(VariableTest, HasFree_SameVariable) {
    EXPECT_TRUE(var_x->has_free("x"));
    EXPECT_FALSE(var_x->has_free("y"));
}

TEST_F(VariableTest, HasFree_DifferentVariable) {
    EXPECT_FALSE(var_y->has_free("x"));
    EXPECT_TRUE(var_y->has_free("y"));
}

TEST_F(VariableTest, SubstituteSameVariable) {
    Variable replacement("z");
    auto result = var_x->substitute("x", replacement);
    Variable* result_var = dynamic_cast<Variable*>(result.get());
    ASSERT_NE(result_var, nullptr);
    EXPECT_EQ(result_var->name, "z");
}

TEST_F(VariableTest, SubstituteDifferentVariable) {
    Variable replacement("z");
    auto result = var_x->substitute("y", replacement);
    Variable* result_var = dynamic_cast<Variable*>(result.get());
    ASSERT_NE(result_var, nullptr);
    EXPECT_EQ(result_var->name, "x");
}

TEST_F(VariableTest, AlphaConvert) {
    auto result = var_x->alpha_convert("newname");
    Variable* result_var = dynamic_cast<Variable*>(result.get());
    ASSERT_NE(result_var, nullptr);
    EXPECT_EQ(result_var->name, "newname");
}

TEST_F(VariableTest, BetaReduceThrowsException) {
    EXPECT_THROW(var_x->beta_reduce(), ReductionOnNormalForm);
}