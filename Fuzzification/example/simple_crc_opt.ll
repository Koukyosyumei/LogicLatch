; ModuleID = 'example/simple_crc.ll'
source_filename = "llvm-link"
target datalayout = "e-m:e-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128"
target triple = "aarch64-unknown-linux-gnu"

%"class.std::ios_base::Init" = type { i8 }
%"class.std::basic_istream" = type { i32 (...)**, i64, %"class.std::basic_ios" }
%"class.std::basic_ios" = type { %"class.std::ios_base", %"class.std::basic_ostream"*, i8, i8, %"class.std::basic_streambuf"*, %"class.std::ctype"*, %"class.std::num_put"*, %"class.std::num_put"* }
%"class.std::ios_base" = type { i32 (...)**, i64, i64, i32, i32, i32, %"struct.std::ios_base::_Callback_list"*, %"struct.std::ios_base::_Words", [8 x %"struct.std::ios_base::_Words"], i32, %"struct.std::ios_base::_Words"*, %"class.std::locale" }
%"struct.std::ios_base::_Callback_list" = type { %"struct.std::ios_base::_Callback_list"*, void (i32, %"class.std::ios_base"*, i32)*, i32, i32 }
%"struct.std::ios_base::_Words" = type { i8*, i64 }
%"class.std::locale" = type { %"class.std::locale::_Impl"* }
%"class.std::locale::_Impl" = type { i32, %"class.std::locale::facet"**, i64, %"class.std::locale::facet"**, i8** }
%"class.std::locale::facet" = type <{ i32 (...)**, i32, [4 x i8] }>
%"class.std::basic_ostream" = type { i32 (...)**, %"class.std::basic_ios" }
%"class.std::basic_streambuf" = type { i32 (...)**, i8*, i8*, i8*, i8*, i8*, i8*, %"class.std::locale" }
%"class.std::ctype" = type <{ %"class.std::locale::facet.base", [4 x i8], %struct.__locale_struct*, i8, [7 x i8], i32*, i32*, i16*, i8, [256 x i8], [256 x i8], i8, [6 x i8] }>
%"class.std::locale::facet.base" = type <{ i32 (...)**, i32 }>
%struct.__locale_struct = type { [13 x %struct.__locale_data*], i16*, i32*, i32*, [13 x i8*] }
%struct.__locale_data = type opaque
%"class.std::num_put" = type { %"class.std::locale::facet.base", [4 x i8] }

@llvm.global_ctors = appending global [1 x { i32, void ()*, i8* }] [{ i32, void ()*, i8* } { i32 65535, void ()* @_GLOBAL__sub_I_simple.cpp, i8* null }]
@_ZStL8__ioinit = internal global %"class.std::ios_base::Init" zeroinitializer, align 1
@__dso_handle = external hidden global i8
@_ZSt3cin = external global %"class.std::basic_istream", align 8
@_ZSt4cout = external global %"class.std::basic_ostream", align 8
@.str = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@.str.1 = private unnamed_addr constant [4 x i8] c"%lf\00", align 1
@.str.2 = private unnamed_addr constant [3 x i8] c"%f\00", align 1

; Function Attrs: noinline uwtable
define internal void @_GLOBAL__sub_I_simple.cpp() #0 section ".text.startup" {
  call void @__cxx_global_var_init()
  ret void
}

; Function Attrs: noinline uwtable
define internal void @__cxx_global_var_init() #0 section ".text.startup" {
  call void @_ZNSt8ios_base4InitC1Ev(%"class.std::ios_base::Init"* noundef nonnull align 1 dereferenceable(1) @_ZStL8__ioinit)
  %1 = call i32 @__cxa_atexit(void (i8*)* bitcast (void (%"class.std::ios_base::Init"*)* @_ZNSt8ios_base4InitD1Ev to void (i8*)*), i8* getelementptr inbounds (%"class.std::ios_base::Init", %"class.std::ios_base::Init"* @_ZStL8__ioinit, i32 0, i32 0), i8* @__dso_handle) #3
  ret void
}

declare void @_ZNSt8ios_base4InitC1Ev(%"class.std::ios_base::Init"* noundef nonnull align 1 dereferenceable(1)) unnamed_addr #1

; Function Attrs: nounwind
declare void @_ZNSt8ios_base4InitD1Ev(%"class.std::ios_base::Init"* noundef nonnull align 1 dereferenceable(1)) unnamed_addr #2

; Function Attrs: nounwind
declare i32 @__cxa_atexit(void (i8*)*, i8*, i8*) #3

; Function Attrs: mustprogress noinline norecurse optnone uwtable
define dso_local noundef i32 @main(i32 noundef %0, i8** noundef %1) #4 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i8**, align 8
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  store i32 0, i32* %3, align 4
  store i32 %0, i32* %4, align 4
  store i8** %1, i8*** %5, align 8
  store i32 0, i32* %6, align 4
  %8 = call noundef nonnull align 8 dereferenceable(16) %"class.std::basic_istream"* @_ZNSirsERi(%"class.std::basic_istream"* noundef nonnull align 8 dereferenceable(16) @_ZSt3cin, i32* noundef nonnull align 4 dereferenceable(4) %6)
  store i32 0, i32* %7, align 4
  %9 = load i32, i32* %6, align 4
  %funcA = call i32 @modifyInt(i32 %9)
  %funcB = call i32 @modifyInt(i32 0)
  %10 = icmp eq i32 %funcA, %funcB
  br i1 %10, label %11, label %12

11:                                               ; preds = %2
  store i32 1, i32* %7, align 4
  br label %12

12:                                               ; preds = %11, %2
  %13 = load i32, i32* %7, align 4
  %14 = call noundef nonnull align 8 dereferenceable(8) %"class.std::basic_ostream"* @_ZNSolsEi(%"class.std::basic_ostream"* noundef nonnull align 8 dereferenceable(8) @_ZSt4cout, i32 noundef %13)
  %15 = call noundef nonnull align 8 dereferenceable(8) %"class.std::basic_ostream"* @_ZNSolsEPFRSoS_E(%"class.std::basic_ostream"* noundef nonnull align 8 dereferenceable(8) %14, %"class.std::basic_ostream"* (%"class.std::basic_ostream"*)* noundef @_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_)
  ret i32 0
}

declare noundef nonnull align 8 dereferenceable(16) %"class.std::basic_istream"* @_ZNSirsERi(%"class.std::basic_istream"* noundef nonnull align 8 dereferenceable(16), i32* noundef nonnull align 4 dereferenceable(4)) #1

declare noundef nonnull align 8 dereferenceable(8) %"class.std::basic_ostream"* @_ZNSolsEi(%"class.std::basic_ostream"* noundef nonnull align 8 dereferenceable(8), i32 noundef) #1

declare noundef nonnull align 8 dereferenceable(8) %"class.std::basic_ostream"* @_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_(%"class.std::basic_ostream"* noundef nonnull align 8 dereferenceable(8)) #1

declare noundef nonnull align 8 dereferenceable(8) %"class.std::basic_ostream"* @_ZNSolsEPFRSoS_E(%"class.std::basic_ostream"* noundef nonnull align 8 dereferenceable(8), %"class.std::basic_ostream"* (%"class.std::basic_ostream"*)* noundef) #1

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local noundef i32 @_Z7reversej(i32 noundef %0) #5 {
  %2 = alloca i32, align 4
  store i32 %0, i32* %2, align 4
  %3 = load i32, i32* %2, align 4
  %4 = and i32 %3, 1431655765
  %5 = shl i32 %4, 1
  %6 = load i32, i32* %2, align 4
  %7 = lshr i32 %6, 1
  %8 = and i32 %7, 1431655765
  %9 = or i32 %5, %8
  store i32 %9, i32* %2, align 4
  %10 = load i32, i32* %2, align 4
  %11 = and i32 %10, 858993459
  %12 = shl i32 %11, 2
  %13 = load i32, i32* %2, align 4
  %14 = lshr i32 %13, 2
  %15 = and i32 %14, 858993459
  %16 = or i32 %12, %15
  store i32 %16, i32* %2, align 4
  %17 = load i32, i32* %2, align 4
  %18 = and i32 %17, 252645135
  %19 = shl i32 %18, 4
  %20 = load i32, i32* %2, align 4
  %21 = lshr i32 %20, 4
  %22 = and i32 %21, 252645135
  %23 = or i32 %19, %22
  store i32 %23, i32* %2, align 4
  %24 = load i32, i32* %2, align 4
  %25 = shl i32 %24, 24
  %26 = load i32, i32* %2, align 4
  %27 = and i32 %26, 65280
  %28 = shl i32 %27, 8
  %29 = or i32 %25, %28
  %30 = load i32, i32* %2, align 4
  %31 = lshr i32 %30, 8
  %32 = and i32 %31, 65280
  %33 = or i32 %29, %32
  %34 = load i32, i32* %2, align 4
  %35 = lshr i32 %34, 24
  %36 = or i32 %33, %35
  store i32 %36, i32* %2, align 4
  %37 = load i32, i32* %2, align 4
  ret i32 %37
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local noundef i32 @_Z5crc32Pc(i8* noundef %0) #5 {
  %2 = alloca i8*, align 8
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  store i8* %0, i8** %2, align 8
  store i32 0, i32* %3, align 4
  store i32 -1, i32* %6, align 4
  br label %7

7:                                                ; preds = %45, %1
  %8 = load i8*, i8** %2, align 8
  %9 = load i32, i32* %3, align 4
  %10 = sext i32 %9 to i64
  %11 = getelementptr inbounds i8, i8* %8, i64 %10
  %12 = load i8, i8* %11, align 1
  %13 = zext i8 %12 to i32
  %funcA = call i32 @modifyInt(i32 %13)
  %funcB = call i32 @modifyInt(i32 0)
  %14 = icmp ne i32 %funcA, %funcB
  br i1 %14, label %15, label %48

15:                                               ; preds = %7
  %16 = load i8*, i8** %2, align 8
  %17 = load i32, i32* %3, align 4
  %18 = sext i32 %17 to i64
  %19 = getelementptr inbounds i8, i8* %16, i64 %18
  %20 = load i8, i8* %19, align 1
  %21 = zext i8 %20 to i32
  store i32 %21, i32* %5, align 4
  %22 = load i32, i32* %5, align 4
  %23 = call noundef i32 @_Z7reversej(i32 noundef %22)
  store i32 %23, i32* %5, align 4
  store i32 0, i32* %4, align 4
  br label %24

24:                                               ; preds = %42, %15
  %25 = load i32, i32* %4, align 4
  %funcA1 = call i32 @modifyInt(i32 %25)
  %funcB2 = call i32 @modifyInt(i32 7)
  %26 = icmp sle i32 %funcA1, %funcB2
  br i1 %26, label %27, label %45

27:                                               ; preds = %24
  %28 = load i32, i32* %6, align 4
  %29 = load i32, i32* %5, align 4
  %30 = xor i32 %28, %29
  %funcA3 = call i32 @modifyInt(i32 %30)
  %funcB4 = call i32 @modifyInt(i32 0)
  %31 = icmp slt i32 %funcA3, %funcB4
  br i1 %31, label %32, label %36

32:                                               ; preds = %27
  %33 = load i32, i32* %6, align 4
  %34 = shl i32 %33, 1
  %35 = xor i32 %34, 79764919
  store i32 %35, i32* %6, align 4
  br label %39

36:                                               ; preds = %27
  %37 = load i32, i32* %6, align 4
  %38 = shl i32 %37, 1
  store i32 %38, i32* %6, align 4
  br label %39

39:                                               ; preds = %36, %32
  %40 = load i32, i32* %5, align 4
  %41 = shl i32 %40, 1
  store i32 %41, i32* %5, align 4
  br label %42

42:                                               ; preds = %39
  %43 = load i32, i32* %4, align 4
  %44 = add nsw i32 %43, 1
  store i32 %44, i32* %4, align 4
  br label %24, !llvm.loop !10

45:                                               ; preds = %24
  %46 = load i32, i32* %3, align 4
  %47 = add nsw i32 %46, 1
  store i32 %47, i32* %3, align 4
  br label %7, !llvm.loop !12

48:                                               ; preds = %7
  %49 = load i32, i32* %6, align 4
  %50 = xor i32 %49, -1
  %51 = call noundef i32 @_Z7reversej(i32 noundef %50)
  ret i32 %51
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i32 @modifyInt(i32 noundef %0) #5 {
  %2 = alloca i32, align 4
  %3 = alloca [200 x i8], align 1
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  store i32 %0, i32* %2, align 4
  %6 = getelementptr inbounds [200 x i8], [200 x i8]* %3, i64 0, i64 0
  %7 = load i32, i32* %2, align 4
  %8 = add nsw i32 %7, 100000
  %9 = call i32 (i8*, i8*, ...) @sprintf(i8* noundef %6, i8* noundef getelementptr inbounds ([3 x i8], [3 x i8]* @.str, i64 0, i64 0), i32 noundef %8) #3
  %10 = getelementptr inbounds [200 x i8], [200 x i8]* %3, i64 0, i64 0
  %11 = call noundef i32 @_Z5crc32Pc(i8* noundef %10)
  store i32 %11, i32* %4, align 4
  %12 = load i32, i32* %4, align 4
  %13 = load i32, i32* %2, align 4
  %14 = sub nsw i32 %12, %13
  store i32 %14, i32* %5, align 4
  %15 = load i32, i32* %4, align 4
  %16 = load i32, i32* %5, align 4
  %17 = sub nsw i32 %15, %16
  ret i32 %17
}

; Function Attrs: nounwind
declare i32 @sprintf(i8* noundef, i8* noundef, ...) #2

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local double @modifyDouble(double noundef %0) #5 {
  %2 = alloca double, align 8
  %3 = alloca [200 x i8], align 1
  %4 = alloca double, align 8
  %5 = alloca double, align 8
  store double %0, double* %2, align 8
  %6 = getelementptr inbounds [200 x i8], [200 x i8]* %3, i64 0, i64 0
  %7 = load double, double* %2, align 8
  %8 = fadd double %7, 1.000000e+05
  %9 = call i32 (i8*, i8*, ...) @sprintf(i8* noundef %6, i8* noundef getelementptr inbounds ([4 x i8], [4 x i8]* @.str.1, i64 0, i64 0), double noundef %8) #3
  %10 = getelementptr inbounds [200 x i8], [200 x i8]* %3, i64 0, i64 0
  %11 = call noundef i32 @_Z5crc32Pc(i8* noundef %10)
  %12 = uitofp i32 %11 to double
  store double %12, double* %4, align 8
  %13 = load double, double* %4, align 8
  %14 = load double, double* %2, align 8
  %15 = fsub double %13, %14
  store double %15, double* %5, align 8
  %16 = load double, double* %4, align 8
  %17 = load double, double* %5, align 8
  %18 = fsub double %16, %17
  ret double %18
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local float @modifyFloat(float noundef %0) #5 {
  %2 = alloca float, align 4
  %3 = alloca [200 x i8], align 1
  %4 = alloca float, align 4
  %5 = alloca float, align 4
  store float %0, float* %2, align 4
  %6 = getelementptr inbounds [200 x i8], [200 x i8]* %3, i64 0, i64 0
  %7 = load float, float* %2, align 4
  %8 = fpext float %7 to double
  %9 = fadd double %8, 1.000000e+05
  %10 = call i32 (i8*, i8*, ...) @sprintf(i8* noundef %6, i8* noundef getelementptr inbounds ([3 x i8], [3 x i8]* @.str.2, i64 0, i64 0), double noundef %9) #3
  %11 = getelementptr inbounds [200 x i8], [200 x i8]* %3, i64 0, i64 0
  %12 = call noundef i32 @_Z5crc32Pc(i8* noundef %11)
  %13 = uitofp i32 %12 to float
  store float %13, float* %4, align 4
  %14 = load float, float* %4, align 4
  %15 = load float, float* %2, align 4
  %16 = fsub float %14, %15
  store float %16, float* %5, align 4
  %17 = load float, float* %4, align 4
  %18 = load float, float* %5, align 4
  %19 = fsub float %17, %18
  ret float %19
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local i32 @retWrapInt(i32 noundef %0) #5 {
  %2 = alloca i32, align 4
  %3 = alloca [200 x i8], align 1
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  store i32 %0, i32* %2, align 4
  %6 = getelementptr inbounds [200 x i8], [200 x i8]* %3, i64 0, i64 0
  %7 = load i32, i32* %2, align 4
  %8 = add nsw i32 %7, 100000
  %9 = call i32 (i8*, i8*, ...) @sprintf(i8* noundef %6, i8* noundef getelementptr inbounds ([3 x i8], [3 x i8]* @.str, i64 0, i64 0), i32 noundef %8) #3
  %10 = getelementptr inbounds [200 x i8], [200 x i8]* %3, i64 0, i64 0
  %11 = call noundef i32 @_Z5crc32Pc(i8* noundef %10)
  store i32 %11, i32* %4, align 4
  %12 = load i32, i32* %4, align 4
  %13 = load i32, i32* %2, align 4
  %14 = sub nsw i32 %12, %13
  store i32 %14, i32* %5, align 4
  %15 = load i32, i32* %4, align 4
  %16 = load i32, i32* %5, align 4
  %17 = sub nsw i32 %15, %16
  ret i32 %17
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local double @retWrapDouble(double noundef %0) #5 {
  %2 = alloca double, align 8
  %3 = alloca [200 x i8], align 1
  %4 = alloca double, align 8
  %5 = alloca double, align 8
  store double %0, double* %2, align 8
  %6 = getelementptr inbounds [200 x i8], [200 x i8]* %3, i64 0, i64 0
  %7 = load double, double* %2, align 8
  %8 = fadd double %7, 1.000000e+05
  %9 = call i32 (i8*, i8*, ...) @sprintf(i8* noundef %6, i8* noundef getelementptr inbounds ([4 x i8], [4 x i8]* @.str.1, i64 0, i64 0), double noundef %8) #3
  %10 = getelementptr inbounds [200 x i8], [200 x i8]* %3, i64 0, i64 0
  %11 = call noundef i32 @_Z5crc32Pc(i8* noundef %10)
  %12 = uitofp i32 %11 to double
  store double %12, double* %4, align 8
  %13 = load double, double* %4, align 8
  %14 = load double, double* %2, align 8
  %15 = fsub double %13, %14
  store double %15, double* %5, align 8
  %16 = load double, double* %4, align 8
  %17 = load double, double* %5, align 8
  %18 = fsub double %16, %17
  ret double %18
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local float @retWrapFloat(float noundef %0) #5 {
  %2 = alloca float, align 4
  %3 = alloca [200 x i8], align 1
  %4 = alloca float, align 4
  %5 = alloca float, align 4
  store float %0, float* %2, align 4
  %6 = getelementptr inbounds [200 x i8], [200 x i8]* %3, i64 0, i64 0
  %7 = load float, float* %2, align 4
  %8 = fpext float %7 to double
  %9 = fadd double %8, 1.000000e+05
  %10 = call i32 (i8*, i8*, ...) @sprintf(i8* noundef %6, i8* noundef getelementptr inbounds ([3 x i8], [3 x i8]* @.str.2, i64 0, i64 0), double noundef %9) #3
  %11 = getelementptr inbounds [200 x i8], [200 x i8]* %3, i64 0, i64 0
  %12 = call noundef i32 @_Z5crc32Pc(i8* noundef %11)
  %13 = uitofp i32 %12 to float
  store float %13, float* %4, align 4
  %14 = load float, float* %4, align 4
  %15 = load float, float* %2, align 4
  %16 = fsub float %14, %15
  store float %16, float* %5, align 4
  %17 = load float, float* %4, align 4
  %18 = load float, float* %5, align 4
  %19 = fsub float %17, %18
  ret float %19
}

declare i8 @modifyBool(i8)

attributes #0 = { noinline uwtable "frame-pointer"="non-leaf" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+neon,+outline-atomics,+v8a" }
attributes #1 = { "frame-pointer"="non-leaf" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+neon,+outline-atomics,+v8a" }
attributes #2 = { nounwind "frame-pointer"="non-leaf" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+neon,+outline-atomics,+v8a" }
attributes #3 = { nounwind }
attributes #4 = { mustprogress noinline norecurse optnone uwtable "frame-pointer"="non-leaf" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+neon,+outline-atomics,+v8a" }
attributes #5 = { mustprogress noinline nounwind optnone uwtable "frame-pointer"="non-leaf" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+neon,+outline-atomics,+v8a" }

!llvm.ident = !{!0, !0}
!llvm.module.flags = !{!1, !2, !3, !4, !5, !6, !7, !8, !9}

!0 = !{!"Ubuntu clang version 14.0.0-1ubuntu1.1"}
!1 = !{i32 1, !"wchar_size", i32 4}
!2 = !{i32 1, !"branch-target-enforcement", i32 0}
!3 = !{i32 1, !"sign-return-address", i32 0}
!4 = !{i32 1, !"sign-return-address-all", i32 0}
!5 = !{i32 1, !"sign-return-address-with-bkey", i32 0}
!6 = !{i32 7, !"PIC Level", i32 2}
!7 = !{i32 7, !"PIE Level", i32 2}
!8 = !{i32 7, !"uwtable", i32 1}
!9 = !{i32 7, !"frame-pointer", i32 1}
!10 = distinct !{!10, !11}
!11 = !{!"llvm.loop.mustprogress"}
!12 = distinct !{!12, !11}
