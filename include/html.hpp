#ifndef HTML_H_
#define HTML_H_ 1

#include <forward_list>
#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <stack>
#include <string>

namespace html {
namespace detail {
    using tag_stack = std::stack<std::string>;

    static tag_stack* stack(std::ostream& os, bool create = true) {
        static int xi = std::ios_base::xalloc();
        auto s = static_cast<tag_stack*>(os.pword(xi));
        if (!s && create) {
            s = new tag_stack;
            os.pword(xi) = s;
            os.register_callback(
                [](std::ios_base::event e, std::ios_base& ios, int) {
                    if (e == std::ios_base::erase_event) {
                        delete static_cast<tag_stack*>(ios.pword(xi));
                    }
                }, 0);
        }
        return s;
    }

    static std::string indent(std::ostream& os) {
        auto s = stack(os);
        size_t sz = 0;
        if (s && os.flags() & std::ios_base::right) {
            sz = s->size() * 2;
        }
        return std::string(sz, ' ');
    }
}  // namespace detail

class attr {
 public:
    attr(const char *name, const char *value)
        : name_(name), value_(value) { }
    attr(const std::string& name, const std::string& value)
        : name_(name), value_(value) { }
    friend std::ostream& operator<<(std::ostream& os, const class attr& a) {
        return os << a.name_ << "=" << std::quoted(a.value_, '\'');
    }

 private:
    const std::string name_;
    const std::string value_;
};

template<typename T>
class declared_attr : public attr {
 public:
    const declared_attr operator()(const char *value) const {
        return declared_attr(value);
    }
    const declared_attr operator()(const std::string value) const {
        return declared_attr(value);
    }

 protected:
    static const std::string ATTR_NAME;
    declared_attr() : declared_attr("") { }

 private:
    explicit declared_attr(const char *value)
        : declared_attr(std::string(value)) { }
    explicit declared_attr(const std::string& value)
        : attr(ATTR_NAME, value) { }
};

template<typename T>
class tag {
 public:
    const tag operator()(const char *content) const {
        return tag(content, attrs_);
    }
    const tag operator()(const std::string content_) const {
        return tag(content_, attrs_);
    }
    tag operator[](const attr& attr) const {
        return tag(content_, { attr });
    }
    tag operator[](const std::initializer_list<attr>& attrs) const {
        return tag(content_, attrs);
    }
    friend std::ostream& operator<<(std::ostream& os, const class tag& t) {
        auto indent = detail::indent(os);
        os << indent << "<" << tag::TAG_NAME;
        for (const auto a : t.attrs_) {
            os << " " << a;
        }
        os << ">";
        if (!t.content_.empty()) {
            os << t.content_ << "</" << tag::TAG_NAME << ">";
        } else {
            detail::stack(os)->push(tag::TAG_NAME);
        }
        if (os.flags() & std::ios_base::right) {
            os << std::endl;
        }
        return os;
    }
    static std::ostream& end(std::ostream& os) {
        auto s = detail::stack(os, false);
        auto matched = false;
        while (s && !s->empty() && !matched) {
            auto name = s->top();
            s->pop();
            auto indent = detail::indent(os);
            os << indent << "</" << name << ">";
            if (os.flags() & std::ios_base::right) {
                os << std::endl;
            }
            matched = tag::TAG_NAME == name;
        }
        return os;
    }

 protected:
    static const std::string TAG_NAME;
    const std::string content_;
    const std::forward_list<class attr> attrs_;
    tag() : tag("") { }

 private:
    explicit tag(const char *content)
        : content_(content), attrs_() { }
    explicit tag(const std::string content)
        : content_(content), attrs_() { }
    tag(const std::string content, const std::forward_list<attr> attrs)
        : content_(content), attrs_(attrs) { }
};

template<typename T>
class empty_tag : public tag<T> {
 public:
    friend std::ostream&
    operator<<(std::ostream& os, const class empty_tag& t) {
        auto indent = detail::indent(os);
        os << indent << "<" << empty_tag::TAG_NAME;
        for (const auto a : t.attrs_) {
            os << " " << a;
        }
        os << "/>";
        if (os.flags() & std::ios_base::right) {
            os << std::endl;
        }
        return os;
    }
};

#define DECLARE_TAG(T) \
    static const class T : public tag<T> { } T; \
    template<> const std::string tag<class T>::TAG_NAME { #T }
DECLARE_TAG(html);
DECLARE_TAG(head);
DECLARE_TAG(script);
DECLARE_TAG(style);
DECLARE_TAG(link);
DECLARE_TAG(meta);
DECLARE_TAG(title);
DECLARE_TAG(body);
DECLARE_TAG(ul);
DECLARE_TAG(ol);
DECLARE_TAG(li);
DECLARE_TAG(dl);
DECLARE_TAG(dd);
DECLARE_TAG(dt);
DECLARE_TAG(table);
DECLARE_TAG(tfoot);
DECLARE_TAG(tbody);
DECLARE_TAG(thead);
DECLARE_TAG(tr);
DECLARE_TAG(th);
DECLARE_TAG(td);
DECLARE_TAG(a);
DECLARE_TAG(b);
DECLARE_TAG(em);
DECLARE_TAG(strong);
DECLARE_TAG(blockquote);
DECLARE_TAG(pre);
DECLARE_TAG(img);
DECLARE_TAG(div);
DECLARE_TAG(span);
DECLARE_TAG(p);
DECLARE_TAG(h1);
DECLARE_TAG(h2);
DECLARE_TAG(h3);
DECLARE_TAG(h4);
DECLARE_TAG(h5);
DECLARE_TAG(h6);
DECLARE_TAG(form);
DECLARE_TAG(label);
DECLARE_TAG(button);
DECLARE_TAG(input);
DECLARE_TAG(select);
DECLARE_TAG(textarea);
#undef DECLARE_TAG

#define DECLARE_EMPTY_TAG(T) \
    static const class T : public empty_tag<T> { } T; \
    template<> const std::string tag<class T>::TAG_NAME { #T }
DECLARE_EMPTY_TAG(br);
DECLARE_EMPTY_TAG(hr);
#undef DECLARE_EMPTY_TAG

#define DECLARE_ATTR(A) \
    static const class A : public declared_attr<A> { } A; \
    template<> const std::string \
    declared_attr<class A>::ATTR_NAME { #A }
DECLARE_ATTR(id);
DECLARE_ATTR(name);
DECLARE_ATTR(charset);
DECLARE_ATTR(content);
DECLARE_ATTR(language);
DECLARE_ATTR(width);
DECLARE_ATTR(height);
DECLARE_ATTR(rel);
DECLARE_ATTR(alt);
DECLARE_ATTR(href);
DECLARE_ATTR(src);
DECLARE_ATTR(action);
DECLARE_ATTR(accept);
DECLARE_ATTR(enctype);
DECLARE_ATTR(method);
DECLARE_ATTR(size);
DECLARE_ATTR(border);
DECLARE_ATTR(cellpadding);
DECLARE_ATTR(colspan);
DECLARE_ATTR(rowspan);
DECLARE_ATTR(target);
DECLARE_ATTR(type);
DECLARE_ATTR(value);
DECLARE_ATTR(selected);
DECLARE_ATTR(rows);
DECLARE_ATTR(cols);
DECLARE_ATTR(wrap);
#undef DECLARE_ATTR

#define DECLARE_ATTR_(A) \
    static const class A##_ : public declared_attr<A##_> { } A##_; \
    template<> const std::string \
    declared_attr<class A##_>::ATTR_NAME { #A }
DECLARE_ATTR_(class);
DECLARE_ATTR_(style);
DECLARE_ATTR_(title);
#undef DECLARE_ATTR_

const auto end = tag<class html>::end;
const auto indent = std::right;

}  // namespace html

#endif
