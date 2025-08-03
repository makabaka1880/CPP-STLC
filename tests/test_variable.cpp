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
    EXPECT_EQ(cloned_var->get_type().to_string(), "τ");
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
    EXPECT_EQ(result_var->get_type().to_string(), "τ");
}

TEST_F(VariableTest, SubstituteDifferentVariable) {
    Variable replacement("z");
    auto result = var_x->substitute("y", replacement);
    Variable* result_var = dynamic_cast<Variable*>(result.get());
    ASSERT_NE(result_var, nullptr);
    EXPECT_EQ(result_var->name, "x");
    EXPECT_EQ(result_var->get_type().to_string(), "τ");
}

TEST_F(VariableTest, AlphaConvert) {
    auto result = var_x->alpha_convert("newname");
    Variable* result_var = dynamic_cast<Variable*>(result.get());
    ASSERT_NE(result_var, nullptr);
    EXPECT_EQ(result_var->name, "newname");
    EXPECT_EQ(result_var->get_type().to_string(), "τ");
}

TEST_F(VariableTest, BetaReduceThrowsException) {
    EXPECT_THROW(var_x->beta_reduce(), ReductionOnNormalForm);
}

TEST_F(VariableTest, DefaultTypeIsTau) {
    Variable var("test");
    EXPECT_EQ(var.get_type().to_string(), "τ");
}

TEST_F(VariableTest, VariableTypeIsTau) {
    EXPECT_EQ(var_x->get_type().to_string(), "τ");
    EXPECT_EQ(var_y->get_type().to_string(), "τ");
}

// Exception tests
TEST_F(VariableTest, BetaReduce_ExceptionMessage) {
    try {
        var_x->beta_reduce();
        FAIL() << "Expected ReductionOnNormalForm exception";
    } catch (const ReductionOnNormalForm& e) {
        std::string message = e.what();
        EXPECT_TRUE(message.find("Reduction on normal form") != std::string::npos);
        EXPECT_TRUE(message.find("x") != std::string::npos);
    }
}

TEST_F(VariableTest, TypeCheck_UndeclaredVariable) {
    TypingContext empty_context;
    EXPECT_THROW(var_x->type_check(empty_context), UndeclaredVariableError);
    
    try {
        var_y->type_check(empty_context);
        FAIL() << "Expected UndeclaredVariableError";
    } catch (const UndeclaredVariableError& e) {
        std::string message = e.what();
        EXPECT_TRUE(message.find("Undeclared variable") != std::string::npos);
        EXPECT_TRUE(message.find("y") != std::string::npos);
    }
}

// Test with different types
TEST_F(VariableTest, Variable_WithCustomTypes) {
    Variable int_var("num", std::make_unique<BaseType>("Int"));
    Variable bool_var("flag", std::make_unique<BaseType>("Bool"));
    Variable string_var("text", std::make_unique<BaseType>("String"));
    
    EXPECT_EQ(int_var.get_type().to_string(), "Int");
    EXPECT_EQ(bool_var.get_type().to_string(), "Bool");
    EXPECT_EQ(string_var.get_type().to_string(), "String");
}

TEST_F(VariableTest, Variable_WithFunctionType) {
    auto func_type = std::make_unique<FunctionType>(
        std::make_unique<BaseType>("Int"),
        std::make_unique<BaseType>("Bool")
    );
    Variable func_var("f", std::move(func_type));
    
    EXPECT_EQ(func_var.get_type().to_string(), "Int -> Bool");
}

TEST_F(VariableTest, Substitute_PreservesCustomType) {
    Variable int_var("x", std::make_unique<BaseType>("Int"));
    Variable replacement("y", std::make_unique<BaseType>("String"));
    
    auto result = int_var.substitute("x", replacement);
    Variable* result_var = dynamic_cast<Variable*>(result.get());
    
    ASSERT_NE(result_var, nullptr);
    EXPECT_EQ(result_var->name, "y");
    EXPECT_EQ(result_var->get_type().to_string(), "String");
}