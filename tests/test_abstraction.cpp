#include <gtest/gtest.h>
#include "../models/terms/Abstraction.h"
#include "../models/terms/Variable.h"
#include "../models/terms/Application.h"
#include "../exceptions/Exceptions.h"

class AbstractionTest : public ::testing::Test {
protected:
    void SetUp() override {
        var_x = std::make_unique<Variable>("x");
        var_y = std::make_unique<Variable>("y");
        
        identity_abs = std::make_unique<Abstraction>(
            std::make_unique<Variable>("x"),
            std::make_unique<Variable>("x")
        );
        
        constant_abs = std::make_unique<Abstraction>(
            std::make_unique<Variable>("x"),
            std::make_unique<Variable>("y")
        );
        
        complex_abs = std::make_unique<Abstraction>(
            std::make_unique<Variable>("f"),
            std::make_unique<Application>(
                std::make_unique<Variable>("f"),
                std::make_unique<Variable>("x")
            )
        );
        
        nested_abs = std::make_unique<Abstraction>(
            std::make_unique<Variable>("x"),
            std::make_unique<Abstraction>(
                std::make_unique<Variable>("y"),
                std::make_unique<Application>(
                    std::make_unique<Variable>("x"),
                    std::make_unique<Variable>("y")
                )
            )
        );
    }

    std::unique_ptr<Variable> var_x;
    std::unique_ptr<Variable> var_y;
    std::unique_ptr<Abstraction> identity_abs;
    std::unique_ptr<Abstraction> constant_abs;
    std::unique_ptr<Abstraction> complex_abs;
    std::unique_ptr<Abstraction> nested_abs;
};

TEST_F(AbstractionTest, Constructor) {
    Abstraction abs(
        std::make_unique<Variable>("x"),
        std::make_unique<Variable>("x")
    );
    
    EXPECT_EQ(abs.var_name, "x");
    Variable* body = dynamic_cast<Variable*>(abs.body.get());
    ASSERT_NE(body, nullptr);
    EXPECT_EQ(body->name, "x");
}

TEST_F(AbstractionTest, Clone) {
    auto cloned = identity_abs->clone();
    Abstraction* cloned_abs = dynamic_cast<Abstraction*>(cloned.get());
    
    ASSERT_NE(cloned_abs, nullptr);
    EXPECT_NE(cloned_abs, identity_abs.get());
    
    EXPECT_EQ(cloned_abs->var_name, "x");
    Variable* cloned_body = dynamic_cast<Variable*>(cloned_abs->body.get());
    ASSERT_NE(cloned_body, nullptr);
    EXPECT_EQ(cloned_body->name, "x");
}

TEST_F(AbstractionTest, HasFree_BoundVariable) {
    EXPECT_FALSE(identity_abs->has_free("x"));
}

TEST_F(AbstractionTest, HasFree_FreeVariable) {
    EXPECT_TRUE(constant_abs->has_free("y"));
    EXPECT_FALSE(constant_abs->has_free("x"));
}

TEST_F(AbstractionTest, HasFree_ComplexBody) {
    EXPECT_TRUE(complex_abs->has_free("x"));
    EXPECT_FALSE(complex_abs->has_free("f"));
}

TEST_F(AbstractionTest, AlphaConvert) {
    auto result = identity_abs->alpha_convert("z");
    Abstraction* result_abs = dynamic_cast<Abstraction*>(result.get());
    
    ASSERT_NE(result_abs, nullptr);
    EXPECT_EQ(result_abs->var_name, "z");
    
    Variable* body = dynamic_cast<Variable*>(result_abs->body.get());
    ASSERT_NE(body, nullptr);
    EXPECT_EQ(body->name, "z");
}

TEST_F(AbstractionTest, AlphaConvert_ComplexBody) {
    auto result = complex_abs->alpha_convert("g");
    Abstraction* result_abs = dynamic_cast<Abstraction*>(result.get());
    
    ASSERT_NE(result_abs, nullptr);
    EXPECT_EQ(result_abs->var_name, "g");
    
    Application* body = dynamic_cast<Application*>(result_abs->body.get());
    ASSERT_NE(body, nullptr);
    
    Variable* func = dynamic_cast<Variable*>(body->function.get());
    Variable* val = dynamic_cast<Variable*>(body->value.get());
    
    ASSERT_NE(func, nullptr);
    ASSERT_NE(val, nullptr);
    EXPECT_EQ(func->name, "g");
    EXPECT_EQ(val->name, "x");
}

TEST_F(AbstractionTest, Substitute_BoundVariable) {
    Variable replacement("replacement");
    auto result = identity_abs->substitute("x", replacement);
    
    Abstraction* result_abs = dynamic_cast<Abstraction*>(result.get());
    ASSERT_NE(result_abs, nullptr);
    
    EXPECT_EQ(result_abs->var_name, "x");
    Variable* body = dynamic_cast<Variable*>(result_abs->body.get());
    ASSERT_NE(body, nullptr);
    EXPECT_EQ(body->name, "x");
}

TEST_F(AbstractionTest, Substitute_FreeVariable) {
    Variable replacement("replacement");
    auto result = constant_abs->substitute("y", replacement);
    
    Abstraction* result_abs = dynamic_cast<Abstraction*>(result.get());
    ASSERT_NE(result_abs, nullptr);
    
    EXPECT_EQ(result_abs->var_name, "x");
    Variable* body = dynamic_cast<Variable*>(result_abs->body.get());
    ASSERT_NE(body, nullptr);
    EXPECT_EQ(body->name, "replacement");
}

TEST_F(AbstractionTest, IsNormal_IdentityFunction) {
    EXPECT_TRUE(identity_abs->is_normal());
}

TEST_F(AbstractionTest, IsNormal_ConstantFunction) {
    EXPECT_TRUE(constant_abs->is_normal());
}

TEST_F(AbstractionTest, IsNormal_ComplexFunction) {
    EXPECT_TRUE(complex_abs->is_normal());
}

TEST_F(AbstractionTest, IsNormal_WithApplication) {
    auto abs_with_app = std::make_unique<Abstraction>(
        std::make_unique<Variable>("x"),
        std::make_unique<Application>(
            std::make_unique<Abstraction>(
                std::make_unique<Variable>("y"),
                std::make_unique<Variable>("y")
            ),
            std::make_unique<Variable>("x")
        )
    );
    
    EXPECT_FALSE(abs_with_app->is_normal());
}

TEST_F(AbstractionTest, BetaReduce_NormalForm) {
    EXPECT_THROW(identity_abs->beta_reduce(), ReductionOnNormalForm);
}

TEST_F(AbstractionTest, BetaReduce_WithReducibleBody) {
    auto abs_with_reducible = std::make_unique<Abstraction>(
        std::make_unique<Variable>("x"),
        std::make_unique<Application>(
            std::make_unique<Abstraction>(
                std::make_unique<Variable>("y"),
                std::make_unique<Variable>("y")
            ),
            std::make_unique<Variable>("z")
        )
    );
    
    auto result = abs_with_reducible->beta_reduce();
    Abstraction* result_abs = dynamic_cast<Abstraction*>(result.get());
    
    ASSERT_NE(result_abs, nullptr);
    EXPECT_EQ(result_abs->var_name, "x");
    
    Variable* body = dynamic_cast<Variable*>(result_abs->body.get());
    ASSERT_NE(body, nullptr);
    EXPECT_EQ(body->name, "z");
}

TEST_F(AbstractionTest, VariableTypeInAbstractionIsTau) {
    EXPECT_EQ(identity_abs->var_type->to_string(), "τ");
    EXPECT_EQ(constant_abs->var_type->to_string(), "τ");
    EXPECT_EQ(complex_abs->var_type->to_string(), "τ");
    EXPECT_EQ(nested_abs->var_type->to_string(), "τ");
}

// Test abstractions with different types
TEST_F(AbstractionTest, Abstraction_WithDifferentTypes) {
    auto int_to_bool_abs = std::make_unique<Abstraction>(
        std::make_unique<Variable>("x", std::make_unique<BaseType>("Int")),
        std::make_unique<Variable>("y", std::make_unique<BaseType>("Bool"))
    );
    
    EXPECT_EQ(int_to_bool_abs->var_type->to_string(), "Int");
    EXPECT_EQ(int_to_bool_abs->var_name, "x");
    
    Variable* body_var = dynamic_cast<Variable*>(int_to_bool_abs->body.get());
    ASSERT_NE(body_var, nullptr);
    EXPECT_EQ(body_var->get_type().to_string(), "Bool");
}

TEST_F(AbstractionTest, Abstraction_FunctionType) {
    auto func_type_var = std::make_unique<Variable>("f", 
        std::make_unique<FunctionType>(
            std::make_unique<BaseType>("Int"),
            std::make_unique<BaseType>("Bool")
        )
    );
    
    auto abs_with_func = std::make_unique<Abstraction>(
        std::move(func_type_var),
        std::make_unique<Variable>("x")
    );
    
    EXPECT_EQ(abs_with_func->var_type->to_string(), "Int -> Bool");
}

// Exception tests
TEST_F(AbstractionTest, BetaReduce_NormalForm_ExceptionMessage) {
    try {
        identity_abs->beta_reduce();
        FAIL() << "Expected ReductionOnNormalForm exception";
    } catch (const ReductionOnNormalForm& e) {
        std::string message = e.what();
        EXPECT_TRUE(message.find("Reduction on normal form") != std::string::npos);
        EXPECT_TRUE(message.find("λ") != std::string::npos);
    }
}

TEST_F(AbstractionTest, TypeCheck_WithTypingContext) {
    TypingContext ctx;
    auto type_int = std::make_unique<BaseType>("Int");
    ctx.add("y", type_int.get());

    // Create the Abstraction: λx:τ. y where y:Int should give τ -> Int
    auto abs = std::make_unique<Abstraction>(
        // Use your robust constructor with a BaseType("τ") for the bound variable x
        std::make_unique<BaseType>("τ"),
        "x",
        std::make_unique<Variable>("y")
    );

    auto func_type = abs->type_check(ctx);
    EXPECT_EQ(func_type->to_string(), "τ -> Int");
}

TEST_F(AbstractionTest, TypeCheck_UndeclaredVariableInBody) {
    TypingContext empty_ctx;
    
    // λx:τ. undefined_var
    auto abs = std::make_unique<Abstraction>(
        std::make_unique<Variable>("x"),
        std::make_unique<Variable>("undefined_var")
    );
    
    EXPECT_THROW(abs->type_check(empty_ctx), UndeclaredVariableError);
}

// Test complex type scenarios
TEST_F(AbstractionTest, Abstraction_NestedFunctionTypes) {
    // Create the types we'll need and store them in unique_ptrs
    auto int_type = std::make_unique<BaseType>("Int");
    auto bool_type = std::make_unique<BaseType>("Bool");
    auto func_type = std::make_unique<FunctionType>(
        int_type->clone(), // Clone since it will be moved
        bool_type->clone() // Clone since it will be moved
    );

    // Create the variables and their types, storing them in unique_ptrs
    auto x_var = std::make_unique<Variable>("x", std::move(int_type));
    auto f_var = std::make_unique<Variable>("f", std::move(func_type));

    // Create the inner body: f(x)
    auto inner_app = std::make_unique<Application>(
        std::make_unique<Variable>("f"),
        std::make_unique<Variable>("x")
    );

    // Create the inner abstraction: λx:Int. f(x)
    auto inner_abs = std::make_unique<Abstraction>(
        std::move(x_var),
        std::move(inner_app)
    );

    // Create the outer abstraction: λf:(Int->Bool). inner_abs
    auto outer_abs = std::make_unique<Abstraction>(
        std::move(f_var),
        std::move(inner_abs)
    );

    TypingContext ctx;
    auto func_type_ = outer_abs->type_check(ctx);
    EXPECT_EQ(func_type_->to_string(), "(Int -> Bool) -> (Int -> Bool)");
}
TEST_F(AbstractionTest, Clone_PreservesTypes) {
    auto int_abs = std::make_unique<Abstraction>(
        std::make_unique<Variable>("x", std::make_unique<BaseType>("Int")),
        std::make_unique<Variable>("y", std::make_unique<BaseType>("String"))
    );
    
    auto cloned = int_abs->clone();
    Abstraction* cloned_abs = dynamic_cast<Abstraction*>(cloned.get());
    
    ASSERT_NE(cloned_abs, nullptr);
    EXPECT_EQ(cloned_abs->var_type->to_string(), "Int");
    EXPECT_EQ(cloned_abs->var_name, "x");
    
    Variable* cloned_body = dynamic_cast<Variable*>(cloned_abs->body.get());
    ASSERT_NE(cloned_body, nullptr);
    EXPECT_EQ(cloned_body->get_type().to_string(), "String");
}