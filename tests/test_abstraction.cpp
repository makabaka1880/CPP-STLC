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
    
    EXPECT_EQ(abs.variable->name, "x");
    Variable* body = dynamic_cast<Variable*>(abs.body.get());
    ASSERT_NE(body, nullptr);
    EXPECT_EQ(body->name, "x");
}

TEST_F(AbstractionTest, Clone) {
    auto cloned = identity_abs->clone();
    Abstraction* cloned_abs = dynamic_cast<Abstraction*>(cloned.get());
    
    ASSERT_NE(cloned_abs, nullptr);
    EXPECT_NE(cloned_abs, identity_abs.get());
    
    EXPECT_EQ(cloned_abs->variable->name, "x");
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
    EXPECT_EQ(result_abs->variable->name, "z");
    
    Variable* body = dynamic_cast<Variable*>(result_abs->body.get());
    ASSERT_NE(body, nullptr);
    EXPECT_EQ(body->name, "z");
}

TEST_F(AbstractionTest, AlphaConvert_ComplexBody) {
    auto result = complex_abs->alpha_convert("g");
    Abstraction* result_abs = dynamic_cast<Abstraction*>(result.get());
    
    ASSERT_NE(result_abs, nullptr);
    EXPECT_EQ(result_abs->variable->name, "g");
    
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
    
    EXPECT_EQ(result_abs->variable->name, "x");
    Variable* body = dynamic_cast<Variable*>(result_abs->body.get());
    ASSERT_NE(body, nullptr);
    EXPECT_EQ(body->name, "x");
}

TEST_F(AbstractionTest, Substitute_FreeVariable) {
    Variable replacement("replacement");
    auto result = constant_abs->substitute("y", replacement);
    
    Abstraction* result_abs = dynamic_cast<Abstraction*>(result.get());
    ASSERT_NE(result_abs, nullptr);
    
    EXPECT_EQ(result_abs->variable->name, "x");
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
    EXPECT_EQ(result_abs->variable->name, "x");
    
    Variable* body = dynamic_cast<Variable*>(result_abs->body.get());
    ASSERT_NE(body, nullptr);
    EXPECT_EQ(body->name, "z");
}