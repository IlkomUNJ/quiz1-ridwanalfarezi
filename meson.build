project('my-cpp-project', 'cpp',
    version: '1.0.0',
    default_options: [
        'cpp_std=c++20',
        'warning_level=3',
        'werror=true', # Treat all warnings as errors
        'optimization=s' # Optimize for size
    ]
)

cpp_args = [
    # Recommended warnings for g++/clang
    '-Wpedantic',
    '-Wformat=2',
    '-Wundef',
    '-Wcast-align',
    '-Wwrite-strings',
    '-Wold-style-cast',
    '-Wnull-dereference',
    '-Wctor-dtor-privacy',
    '-Wno-missing-field-initializers',
]

add_global_arguments(cpp_args, language : 'cpp')

app_sources = [
    'main.cpp',
    'bank_customer/bank_customer.cpp',
    'bank_customer/bank_customer.h',
    'order/order.cpp',
    'order/order.h',
    'store/store.cpp',
    'store/store.h',
    'buyer/buyer.cpp',
    'buyer/buyer.h',
    'seller/seller.cpp',
    'seller/seller.h',
    'bank/bank.h',
    'item/item.h',
    'transaction/transaction.h'
]

executable('my_app',
    app_sources,
    # Example dependencies:
    # dependencies: [fmt_dep],
    link_args: ['-static', '-static-libgcc', '-static-libstdc++'],
    install: true
)