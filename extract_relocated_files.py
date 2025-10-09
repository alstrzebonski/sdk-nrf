#!/usr/bin/env python3
"""
Script to extract information about relocated files from the Zephyr linker map file.
This script identifies files that have been relocated to RAM using zephyr_code_relocate().
"""

import re
import sys
from pathlib import Path
from collections import defaultdict

def extract_relocated_files(map_file_path, ram_start_addr=0x20000000, ram_end_addr=0x30000000):
    """
    Extract information about files relocated to RAM from the linker map file.
    
    Args:
        map_file_path: Path to the zephyr.map file
        ram_start_addr: Start address of RAM region (default for nRF)
        ram_end_addr: End address of RAM region (generous upper bound)
    
    Returns:
        Dictionary with relocated file information
    """
    relocated_functions = []
    relocated_files = defaultdict(list)
    
    with open(map_file_path, 'r') as f:
        lines = f.readlines()
    
    # Pattern to match function/symbol entries with address and source info
    # Format: " .text.function_name"
    #         "                0x20xxxxxx       0xsize path/to/lib.a(source.c.obj)"
    text_pattern = re.compile(r'^\s+\.text\.(\S+)')
    addr_pattern = re.compile(r'^\s+(0x[0-9a-fA-F]+)\s+(0x[0-9a-fA-F]+)\s+(.+)$')
    
    i = 0
    while i < len(lines):
        line = lines[i].rstrip()
        
        # Look for .text.function_name entries
        text_match = text_pattern.match(line)
        if text_match:
            function_name = text_match.group(1)
            
            # Check the next line for address information
            if i + 1 < len(lines):
                next_line = lines[i + 1].rstrip()
                addr_match = addr_pattern.match(next_line)
                
                if addr_match:
                    addr_str = addr_match.group(1)
                    size_str = addr_match.group(2)
                    obj_file = addr_match.group(3)
                    
                    try:
                        addr = int(addr_str, 16)
                        size = int(size_str, 16)
                        
                        # Check if this is in RAM (relocated code)
                        if ram_start_addr <= addr < ram_end_addr:
                            relocated_functions.append({
                                'function': function_name,
                                'address': addr_str,
                                'size': size_str,
                                'size_bytes': size,
                                'object_file': obj_file
                            })
                            
                            # Extract source file information from object file path
                            source_file = extract_source_file_from_obj_path(obj_file)
                            if source_file:
                                relocated_files[source_file].append({
                                    'function': function_name,
                                    'address': addr_str,
                                    'size_bytes': size,
                                    'object_file': obj_file
                                })
                    except ValueError:
                        pass
        
        i += 1
    
    return relocated_functions, dict(relocated_files)

def extract_source_file_from_obj_path(obj_file_path):
    """
    Extract the source file name from the object file path in the map file.
    
    Examples:
    - "app/libapp.a(hids.c.obj)" -> "hids.c"
    - "zephyr/drivers/gpio/libdrivers__gpio.a(gpio_nrfx.c.obj)" -> "gpio_nrfx.c"
    - "/tmp/ccPzkdQf.ltrans4.ltrans.o" -> "lto_optimized"
    """
    # Pattern to match library.a(source.c.obj) format
    lib_pattern = re.compile(r'.*\.a\(([^)]+)\.obj\)')
    match = lib_pattern.search(obj_file_path)
    
    if match:
        source_name = match.group(1)
        # Handle cases like "source.c" -> keep as is
        # Handle cases like "source" -> add .c extension if it looks like a C file
        if not source_name.endswith(('.c', '.cpp', '.cc', '.cxx')):
            # Check if it's likely a C source file
            if not source_name.endswith(('.h', '.hpp', '.s', '.S', '.asm')):
                source_name += '.c'
        return source_name
    
    # Handle LTO optimized files (temporary compiler files)
    if '/tmp/cc' in obj_file_path and 'ltrans' in obj_file_path:
        return 'lto_optimized'
    
    # Handle other object files
    if obj_file_path.endswith('.o') or obj_file_path.endswith('.obj'):
        # Try to extract filename from path
        filename = Path(obj_file_path).stem
        if filename.startswith('cc') and 'ltrans' in filename:
            return 'lto_optimized'
        return filename + '.c'
    
    return 'unknown'

def print_summary(relocated_functions, relocated_files):
    """Print a summary of relocated files and functions."""
    print("=== RELOCATED FILES SUMMARY ===")
    print(f"Total relocated functions: {len(relocated_functions)}")
    print(f"Total relocated files: {len(relocated_files)}")
    print()
    
    # Calculate total size
    total_size = sum(func['size_bytes'] for func in relocated_functions)
    print(f"Total relocated code size: {total_size} bytes ({total_size/1024:.1f} KB)")
    print()
    
    # Print by file
    print("=== BY SOURCE FILE ===")
    for source_file, functions in sorted(relocated_files.items()):
        file_size = sum(func['size_bytes'] for func in functions)
        print(f"\n{source_file}: {len(functions)} functions, {file_size} bytes")
        for func in sorted(functions, key=lambda x: int(x['address'], 16)):
            print(f"  {func['address']}: {func['function']} ({func['size_bytes']} bytes)")
            # Show object file for first few entries as examples
            if functions.index(func) < 2:  # Show object file for first 2 functions
                print(f"    from: {func['object_file']}")
    
    print("\n=== ALL RELOCATED FUNCTIONS (by address) ===")
    for func in sorted(relocated_functions, key=lambda x: int(x['address'], 16))[:20]:  # Show first 20
        print(f"{func['address']}: {func['function']} ({func['size_bytes']} bytes)")
    
    if len(relocated_functions) > 20:
        print(f"... and {len(relocated_functions) - 20} more functions")

def filter_by_regex(relocated_functions, relocated_files, pattern):
    """Filter results by regex pattern matching function names or file names."""
    import re
    regex = re.compile(pattern, re.IGNORECASE)
    
    # Filter functions
    filtered_functions = [
        func for func in relocated_functions 
        if regex.search(func['function']) or regex.search(func['object_file'])
    ]
    
    # Filter files
    filtered_files = {}
    for file_name, functions in relocated_files.items():
        if regex.search(file_name):
            filtered_files[file_name] = functions
        else:
            # Check if any function in this file matches
            matching_functions = [
                func for func in functions 
                if regex.search(func['function'])
            ]
            if matching_functions:
                filtered_files[file_name] = matching_functions
    
    return filtered_functions, filtered_files

def main():
    import argparse
    
    parser = argparse.ArgumentParser(
        description="Extract relocated files from Zephyr linker map file",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  # Show all relocated files
  python3 extract_relocated_files.py build/zephyr/zephyr.map
  
  # Filter by regex pattern
  python3 extract_relocated_files.py build/zephyr/zephyr.map --filter "gpio|led"
  
  # Show only specific file types
  python3 extract_relocated_files.py build/zephyr/zephyr.map --filter "\.c$"
        """
    )
    
    parser.add_argument('map_file', help='Path to the zephyr.map file')
    parser.add_argument('--filter', '-f', help='Regex pattern to filter results')
    parser.add_argument('--ram-start', type=lambda x: int(x, 0), default=0x20000000,
                       help='RAM start address (default: 0x20000000)')
    parser.add_argument('--ram-end', type=lambda x: int(x, 0), default=0x30000000,
                       help='RAM end address (default: 0x30000000)')
    
    args = parser.parse_args()
    
    if not Path(args.map_file).exists():
        print(f"Error: Map file {args.map_file} not found")
        sys.exit(1)
    
    print(f"Analyzing map file: {args.map_file}")
    print(f"Looking for code relocated to RAM (0x{args.ram_start:08x} - 0x{args.ram_end:08x})...")
    if args.filter:
        print(f"Filtering by pattern: {args.filter}")
    print()
    
    relocated_functions, relocated_files = extract_relocated_files(
        args.map_file, args.ram_start, args.ram_end
    )
    
    if args.filter:
        relocated_functions, relocated_files = filter_by_regex(
            relocated_functions, relocated_files, args.filter
        )
        print(f"=== FILTERED RESULTS (pattern: {args.filter}) ===")
    
    print_summary(relocated_functions, relocated_files)

if __name__ == "__main__":
    main()
