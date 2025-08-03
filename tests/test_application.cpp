#include <gtest/gtest.h>
#include "../models/terms/Application.h"
#include "../models/terms/Variable.h"
#include "../models/terms/Abstraction.h"
#include "../exceptions/Exceptions.h"

class ApplicationTest : public ::testing::Test {
protected:
    void SetUp() override {
        var_x = std::make_unique<Variable>("x");
        var_y = std::make_unique<Variable>("y");
        var_z = std::make_unique<Variable>("z");
        
        simple_app = std::make_unique<Application>(
            std::make_unique<Variable>("f"),
            std::make_unique<Variable>("x")
        );
        
        identity_func = std::make_unique<Abstraction>(
            std::make_unique<Variable>("x"),
            std::make_unique<Variable>("x")
        );
        
        app_with_abstraction = std::make_unique<Application>(
            std::make_unique<Abstraction>(
                std::make_unique<Variable>("x"),
                std::make_unique<Variable>("x")
            ),
            std::make_unique<Variable>("y")
        );
    }

    std::unique_ptr<Variable> var_x;
    std::unique_ptr<Variable> var_y;
    std::unique_ptr<Variable> var_z;
    std::unique_ptr<Application> simple_app;
    std::unique_ptr<Abstraction> identity_func;
    std::unique_ptr<Application> app_with_abstraction;
};

TEST_F(ApplicationTest, Constructor) {
    Application app(
        std::make_unique<Variable>("f"),
        std::make_unique<Variable>("x")
    );
    
    Variable* func = dynamic_cast<Variable*>(app.function.get());
    Variable* val = dynamic_cast<Variable*>(app.value.get());
    
    ASSERT_NE(func, nullptr);
    ASSERT_NE(val, nullptr);
    EXPECT_EQ(func->name, "f");
    EXPECT_EQ(val->name, "x");
}

TEST_F(ApplicationTest, Clone) {
    auto cloned = simple_app->clone();
    Application* cloned_app = dynamic_cast<Application*>(cloned.get());
    
    ASSERT_NE(cloned_app, nullptr);
    EXPECT_NE(cloned_app, simple_app.get());
    
    Variable* cloned_func = dynamic_cast<Variable*>(cloned_app->function.get());
    Variable* cloned_val = dynamic_cast<Variable*>(cloned_app->value.get());
    
    ASSERT_NE(cloned_func, nullptr);
    ASSERT_NE(cloned_val, nullptr);
    EXPECT_EQ(cloned_func->name, "f");
    EXPECT_EQ(cloned_val->name, "x");
}

TEST_F(ApplicationTest, HasFree_BothHaveFree) {
    EXPECT_TRUE(simple_app->has_free("f"));
    EXPECT_TRUE(simple_app->has_free("x"));
    EXPECT_FALSE(simple_app->has_free("y"));
}

TEST_F(ApplicationTest, HasFree_OnlyFunctionHasFree) {
    Application app(
        std::make_unique<Variable>("f"),
        std::make_unique<Abstraction>(
            std::make_unique<Variable>("x"),
            std::make_unique<Variable>("x")
        )
    );
    
    EXPECT_TRUE(app.has_free("f"));
    EXPECT_FALSE(app.has_free("x"));
}

TEST_F(ApplicationTest, AlphaConvert) {
    auto result = simple_app->alpha_convert("newvar");
    Application* result_app = dynamic_cast<Application*>(result.get());
    
    ASSERT_NE(result_app, nullptr);
    
    Variable* func = dynamic_cast<Variable*>(result_app->function.get());
    Variable* val = dynamic_cast<Variable*>(result_app->value.get());
    
    ASSERT_NE(func, nullptr);
    ASSERT_NE(val, nullptr);
    EXPECT_EQ(func->name, "newvar");
    EXPECT_EQ(val->name, "newvar");
}

TEST_F(ApplicationTest, Substitute) {
    Variable replacement("replacement");
    auto result = simple_app->substitute("f", replacement);
    Application* result_app = dynamic_cast<Application*>(result.get());
    
    ASSERT_NE(result_app, nullptr);
    
    Variable* func = dynamic_cast<Variable*>(result_app->function.get());
    Variable* val = dynamic_cast<Variable*>(result_app->value.get());
    
    ASSERT_NE(func, nullptr);
    ASSERT_NE(val, nullptr);
    EXPECT_EQ(func->name, "replacement");
    EXPECT_EQ(val->name, "x");
}

TEST_F(ApplicationTest, IsNormal_SimpleApplication) {
    EXPECT_TRUE(simple_app->is_normal());
}

TEST_F(ApplicationTest, IsNormal_ApplicationWithAbstraction) {
    EXPECT_FALSE(app_with_abstraction->is_normal());
}

TEST_F(ApplicationTest, BetaReduce_WithAbstraction) {
    auto result = app_with_abstraction->beta_reduce();
    Variable* result_var = dynamic_cast<Variable*>(result.get());
    
    ASSERT_NE(result_var, nullptr);
    EXPECT_EQ(result_var->name, "y");
}

TEST_F(ApplicationTest, BetaReduce_NormalForm) {
    EXPECT_THROW(simple_app->beta_reduce(), ReductionOnNormalForm);
}

TEST_F(ApplicationTest, BetaReduce_ReduceFunction) {
    auto nested_app = std::make_unique<Application>(
        std::make_unique<Application>(
            std::make_unique<Abstraction>(
                std::make_unique<Variable>("x"),
                std::make_unique<Variable>("x")
            ),
            std::make_unique<Variable>("f")
        ),
        std::make_unique<Variable>("y")
    );
    
    auto result = nested_app->beta_reduce();
    Application* result_app = dynamic_cast<Application*>(result.get());
    
    ASSERT_NE(result_app, nullptr);
    
    Variable* func = dynamic_cast<Variable*>(result_app->function.get());
    Variable* val = dynamic_cast<Variable*>(result_app->value.get());
    
    ASSERT_NE(func, nullptr);
    ASSERT_NE(val, nullptr);
    EXPECT_EQ(func->name, "f");
    EXPECT_EQ(val->name, "y");
}