#include <html.hpp>
#include <sstream>
#include <gtest/gtest.h>

TEST(htmlpp, body) {
    std::ostringstream os;
    os << html::html << html::body << html::end;
    ASSERT_EQ(os.str(), "<html><body></body></html>");
}

TEST(htmlpp, body_text) {
    std::ostringstream os;
    os << html::html << html::body << "test" << html::end;
    ASSERT_EQ(os.str(), "<html><body>test</body></html>");
}

TEST(htmlpp, end_tag_partial) {
    std::ostringstream os;
    os << html::html << html::body << "test" << html::body::end;
    ASSERT_EQ(os.str(), "<html><body>test</body>");
}

TEST(htmlpp, end_no_tags) {
    std::ostringstream os;
    os << html::end;
    ASSERT_EQ(os.str(), "");
}

TEST(htmlpp, empty_tags) {
    std::ostringstream os;
    os << html::hr;
    ASSERT_EQ(os.str(), "<hr/>");
}

TEST(htmlpp, str_content) {
    std::ostringstream os;
    os << html::html("text");
    ASSERT_EQ(os.str(), "<html>text</html>");
}

TEST(htmlpp, generic_attrs) {
    std::ostringstream os;
    os << html::body[html::attr("id", "test")] << html::end;
    ASSERT_EQ(os.str(), "<body id='test'></body>");
    os.str("");
    os << html::body << html::end;
    ASSERT_EQ(os.str(), "<body></body>");
}

TEST(htmlpp, declared_attrs) {
    std::ostringstream os;
    os << html::a[html::href("#")]("link") << html::end;
    ASSERT_EQ(os.str(), "<a href='#'>link</a>");
}

TEST(htmlpp, attrs_with_name_conflicts) {
    std::ostringstream os;
    os << html::div[html::class_("test")] << html::end;
    ASSERT_EQ(os.str(), "<div class='test'></div>");
}

TEST(htmlpp, multiple_attrs) {
    std::ostringstream os;
    os << html::a[{
            html::id("foo"),
            html::href("#"),
            html::attr("a", "b") }]("link")
        << html::end;
    ASSERT_EQ(os.str(), "<a id='foo' href='#' a='b'>link</a>");
}

TEST(htmlpp, indenting) {
    std::ostringstream os;
    os << html::indent << html::html << html::body << html::end;
    ASSERT_EQ(os.str(),
        "<html>\n"
        "  <body>\n"
        "  </body>\n"
        "</html>\n"
    );
}

TEST(htmlpp, complete_example) {
    std::ostringstream os;
    os << html::indent
        << html::html
        << html::body
        << html::a[html::href("#")]("testing")
        << html::end;
    ASSERT_EQ(
        os.str(),
        "<html>\n"
        "  <body>\n"
        "    <a href='#'>testing</a>\n"
        "  </body>\n"
        "</html>\n"
    );
}

