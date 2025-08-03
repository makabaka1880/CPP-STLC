#include <gtest/gtest.h>
#include "../models/Type.h"
#include "../models/TypingContext.h"
#include "../models/terms/Variable.h"
#include "../models/terms/Abstraction.h"
#include "../models/terms/Application.h"
#include "../exceptions/Exceptions.h"

class TypesAndExceptionsTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create various base types
        tau_type = std::make_unique<BaseType>("τ");
        int_type = std::make_unique<BaseType>("Int");
        bool_type = std::make_unique<BaseType>("Bool");
        string_type = std::make_unique<BaseType>("String");
        
        // Create function types
        int_to_int = std::make_unique<FunctionType>(
            std::make_unique<BaseType>("Int"),
            std::make_unique<BaseType>("Int")
        );
        
        int_to_bool = std::make_unique<FunctionType>(
            std::make_unique<BaseType>("Int"),
            std::make_unique<BaseType>("Bool")
        );
        
        // Complex nested function type: (Int -> Bool) -> (String -> Int)
        nested_function_type = std::make_unique<FunctionType>(
            std::make_unique<FunctionType>(
                std::make_unique<BaseType>("Int"),
                std::make_unique<BaseType>("Bool")
            ),
            std::make_unique<FunctionType>(
                std::make_unique<BaseType>("String"),
                std::make_unique<BaseType>("Int")
            )
        );
        
        // Create variables with different types
        var_tau = std::make_unique<Variable>("x");  // defaults to τ
        var_int = std::make_unique<Variable>("y", std::make_unique<BaseType>("Int"));
        var_bool = std::make_unique<Variable>("z", std::make_unique<BaseType>("Bool"));
        
        // Create typing context
        context = std::make_unique<TypingContext>();
        context->add("x", tau_type.get());
        context->add("y", int_type.get());
        context->add("z", bool_type.get());
    }

    std::unique_ptr<BaseType> tau_type;
    std::unique_ptr<BaseType> int_type;
    std::unique_ptr<BaseType> bool_type;
    std::unique_ptr<BaseType> string_type;
    std::unique_ptr<FunctionType> int_to_int;
    std::unique_ptr<FunctionType> int_to_bool;
    std::unique_ptr<FunctionType> nested_function_type;
    
    std::unique_ptr<Variable> var_tau;
    std::unique_ptr<Variable> var_int;
    std::unique_ptr<Variable> var_bool;
    
    std::unique_ptr<TypingContext> context;
};

// Test BaseType functionality
TEST_F(TypesAndExceptionsTest, BaseType_ToString) {
    EXPECT_EQ(tau_type->to_string(), "τ");
    EXPECT_EQ(int_type->to_string(), "Int");
    EXPECT_EQ(bool_type->to_string(), "Bool");
    EXPECT_EQ(string_type->to_string(), "String");
}

TEST_F(TypesAndExceptionsTest, BaseType_Clone) {
    auto cloned_tau = tau_type->clone();
    auto cloned_int = int_type->clone();
    
    EXPECT_EQ(cloned_tau->to_string(), "τ");
    EXPECT_EQ(cloned_int->to_string(), "Int");
    EXPECT_NE(cloned_tau.get(), tau_type.get());
    EXPECT_NE(cloned_int.get(), int_type.get());
}

// Test FunctionType functionality
TEST_F(TypesAndExceptionsTest, FunctionType_ToString) {
    EXPECT_EQ(int_to_int->to_string(), "Int -> Int");
    EXPECT_EQ(int_to_bool->to_string(), "Int -> Bool");
    EXPECT_EQ(nested_function_type->to_string(), "(Int -> Bool) -> (String -> Int)");
}

TEST_F(TypesAndExceptionsTest, FunctionType_Clone) {
    auto cloned = int_to_int->clone();
    EXPECT_EQ(cloned->to_string(), "Int -> Int");
    EXPECT_NE(cloned.get(), int_to_int.get());
}

TEST_F(TypesAndExceptionsTest, FunctionType_CopyConstructor) {
    FunctionType copied(*int_to_bool);
    EXPECT_EQ(copied.to_string(), "Int -> Bool");
}

// Test Variable types
TEST_F(TypesAndExceptionsTest, Variable_DefaultType) {
    Variable default_var("test");
    EXPECT_EQ(default_var.get_type().to_string(), "τ");
}

TEST_F(TypesAndExceptionsTest, Variable_ExplicitTypes) {
    EXPECT_EQ(var_tau->get_type().to_string(), "τ");
    EXPECT_EQ(var_int->get_type().to_string(), "Int");
    EXPECT_EQ(var_bool->get_type().to_string(), "Bool");
}

TEST_F(TypesAndExceptionsTest, Variable_TypePreservedInOperations) {
    auto cloned_int = var_int->clone();
    auto alpha_converted_bool = var_bool->alpha_convert("w");
    
    Variable* cloned_var = dynamic_cast<Variable*>(cloned_int.get());
    Variable* alpha_var = dynamic_cast<Variable*>(alpha_converted_bool.get());
    
    ASSERT_NE(cloned_var, nullptr);
    ASSERT_NE(alpha_var, nullptr);
    
    EXPECT_EQ(cloned_var->get_type().to_string(), "Int");
    EXPECT_EQ(alpha_var->get_type().to_string(), "Bool");
}

// Test TypingContext
TEST_F(TypesAndExceptionsTest, TypingContext_Lookup) {
    EXPECT_EQ(context->lookup("x")->to_string(), "τ");
    EXPECT_EQ(context->lookup("y")->to_string(), "Int");
    EXPECT_EQ(context->lookup("z")->to_string(), "Bool");
    EXPECT_EQ(context->lookup("nonexistent"), nullptr);
}

// Test Variable type checking
TEST_F(TypesAndExceptionsTest, Variable_TypeCheck_Success) {
    auto type_x = var_tau->type_check(*context);
    auto type_y = var_int->type_check(*context);
    auto type_z = var_bool->type_check(*context);
    
    EXPECT_EQ(type_x->to_string(), "τ");
    EXPECT_EQ(type_y->to_string(), "Int");
    EXPECT_EQ(type_z->to_string(), "Bool");
}

TEST_F(TypesAndExceptionsTest, Variable_TypeCheck_UndeclaredVariable) {
    Variable undeclared("undeclared");
    EXPECT_THROW(undeclared.type_check(*context), UndeclaredVariableError);
}

// Test Abstraction types
TEST_F(TypesAndExceptionsTest, Abstraction_SimpleType) {
    // λx:τ. x : τ -> τ
    auto identity = std::make_unique<Abstraction>(
        std::make_unique<Variable>("x"),
        std::make_unique<Variable>("x")
    );
    
    auto func_type = identity->type_check(*context);
    EXPECT_EQ(func_type->to_string(), "τ -> τ");
}

TEST_F(TypesAndExceptionsTest, Abstraction_WithDifferentTypes) {
    // λx:Int. x : Int -> Int
    auto int_identity = std::make_unique<Abstraction>(
        std::make_unique<Variable>("x", std::make_unique<BaseType>("Int")),
        std::make_unique<Variable>("x")
    );
    
    auto func_type = int_identity->type_check(*context);
    EXPECT_EQ(func_type->to_string(), "Int -> Int");
}

TEST_F(TypesAndExceptionsTest, Abstraction_NestedAbstraction) {
    // λx:Int. λy:Bool. x : Int -> (Bool -> Int)
    auto nested = std::make_unique<Abstraction>(
        std::make_unique<Variable>("x", std::make_unique<BaseType>("Int")),
        std::make_unique<Abstraction>(
            std::make_unique<Variable>("y", std::make_unique<BaseType>("Bool")),
            std::make_unique<Variable>("x")
        )
    );
    
    auto func_type = nested->type_check(*context);
    EXPECT_EQ(func_type->to_string(), "Int -> (Bool -> Int)");
}

// Exception Tests
TEST_F(TypesAndExceptionsTest, ReductionOnNormalForm_Exception) {
    Variable normal_var("x");
    
    try {
        normal_var.beta_reduce();
        FAIL() << "Expected ReductionOnNormalForm exception";
    } catch (const ReductionOnNormalForm& e) {
        std::string message = e.what();
        EXPECT_TRUE(message.find("Reduction on normal form") != std::string::npos);
        EXPECT_TRUE(message.find("x") != std::string::npos);
    }
}

TEST_F(TypesAndExceptionsTest, UndeclaredVariableError_Exception) {
    Variable undeclared("undefined");
    
    try {
        undeclared.type_check(*context);
        FAIL() << "Expected UndeclaredVariableError exception";
    } catch (const UndeclaredVariableError& e) {
        std::string message = e.what();
        EXPECT_TRUE(message.find("Undeclared variable") != std::string::npos);
        EXPECT_TRUE(message.find("undefined") != std::string::npos);
    }
}

TEST_F(TypesAndExceptionsTest, TypeMismatchError_BaseClass) {
    try {
        throw TypeMismatchError("Test type mismatch");
    } catch (const TypeMismatchError& e) {
        EXPECT_STREQ(e.what(), "Test type mismatch");
    }
}

TEST_F(TypesAndExceptionsTest, NotAFunctionError_Exception) {
    try {
        throw NotAFunctionError("Int");
    } catch (const NotAFunctionError& e) {
        std::string message = e.what();
        EXPECT_TRUE(message.find("not a function") != std::string::npos);
        EXPECT_TRUE(message.find("Int") != std::string::npos);
    }
}

TEST_F(TypesAndExceptionsTest, DomainTypeMismatchError_Exception) {
    try {
        throw DomainTypeMismatchError("Int", "Bool");
    } catch (const DomainTypeMismatchError& e) {
        std::string message = e.what();
        EXPECT_TRUE(message.find("Type mismatch") != std::string::npos);
        EXPECT_TRUE(message.find("Int") != std::string::npos);
        EXPECT_TRUE(message.find("Bool") != std::string::npos);
    }
}

// Test exception inheritance
TEST_F(TypesAndExceptionsTest, Exception_Inheritance) {
    NotAFunctionError not_func("Test");
    DomainTypeMismatchError domain_mismatch("A", "B");
    
    // Both should be catchable as TypeMismatchError
    EXPECT_NO_THROW({
        try {
            throw not_func;
        } catch (const TypeMismatchError& e) {
            // Should catch successfully
        }
    });
    
    EXPECT_NO_THROW({
        try {
            throw domain_mismatch;
        } catch (const TypeMismatchError& e) {
            // Should catch successfully
        }
    });
}

// Test complex type scenarios
TEST_F(TypesAndExceptionsTest, ComplexTypes_DeepNesting) {
    // ((Int -> Bool) -> String) -> ((Bool -> Int) -> Char)
    auto very_nested = std::make_unique<FunctionType>(
        std::make_unique<FunctionType>(
            std::make_unique<FunctionType>(
                std::make_unique<BaseType>("Int"),
                std::make_unique<BaseType>("Bool")
            ),
            std::make_unique<BaseType>("String")
        ),
        std::make_unique<FunctionType>(
            std::make_unique<FunctionType>(
                std::make_unique<BaseType>("Bool"),
                std::make_unique<BaseType>("Int")
            ),
            std::make_unique<BaseType>("Char")
        )
    );
    
    std::string expected = "((Int -> Bool) -> String) -> ((Bool -> Int) -> Char)";
    EXPECT_EQ(very_nested->to_string(), expected);
}

TEST_F(TypesAndExceptionsTest, Variable_Substitution_TypePreservation) {
    Variable replacement("replacement", std::make_unique<BaseType>("String"));
    auto result = var_int->substitute("y", replacement);
    
    Variable* result_var = dynamic_cast<Variable*>(result.get());
    ASSERT_NE(result_var, nullptr);
    EXPECT_EQ(result_var->get_type().to_string(), "String");
}

TEST_F(TypesAndExceptionsTest, Multiple_Exception_Types_In_Same_Context) {
    // Test that we can have multiple different exception scenarios
    TypingContext empty_context;
    Variable var1("missing1");
    Variable var2("missing2");
    
    EXPECT_THROW(var1.type_check(empty_context), UndeclaredVariableError);
    EXPECT_THROW(var2.type_check(empty_context), UndeclaredVariableError);
    EXPECT_THROW(var1.beta_reduce(), ReductionOnNormalForm);
    EXPECT_THROW(var2.beta_reduce(), ReductionOnNormalForm);
}