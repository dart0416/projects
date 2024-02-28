def ip_to_int(ip):
    #Konwertuje adres IP z notacji kropkowej na liczbę całkowitą
    octets = map(int, ip.split('.'))
    return sum([octet << (8 * (3 - i)) for i, octet in enumerate(octets)])

def int_to_ip(int_val):
    #Konwertuje liczbę całkowitą na adres IP w notacji kropkowej
    return '.'.join(str((int_val >> (8 * i)) & 0xFF) for i in reversed(range(4)))

def get_broadcast_address(network_address, subnet_mask):
    #Oblicza adres broadcast na podstawie adresu sieci i maski
    network_int = ip_to_int(network_address)
    mask_int = ip_to_int(subnet_mask)
    broadcast_int = network_int | ~mask_int & 0xFFFFFFFF
    return int_to_ip(broadcast_int)

def get_network_address(ip, subnet_mask):
    #Oblicza adres sieci na podstawie adresu IP i maski
    ip_int = ip_to_int(ip)
    mask_int = ip_to_int(subnet_mask)
    network_int = ip_int & mask_int
    return int_to_ip(network_int)

def subnet_mask_from_prefix_length(prefix_length):
    #Zwraca maskę podsieci w notacji kropkowej na podstawie długości prefiksu
    mask_int = (0xFFFFFFFF >> (32 - prefix_length)) << (32 - prefix_length)
    return int_to_ip(mask_int)

def calculate_subnets(base_network, base_mask, sizes):
    #Dzieli sieć na podsieci zgodnie z wymaganymi rozmiarami
    sizes.sort(reverse=True)  # Sortowanie rozmiarów od największego do najmniejszego
    subnets = []
    current_base = ip_to_int(base_network)
    subnet_number = 1
    
    for size in sizes:
        # Oblicz potrzebną długość maski dla danej wielkości podsieci
        new_prefix_length = 32
        while 2 ** (32 - new_prefix_length) < size + 2:  # +2 dla adresu sieci i broadcast
            new_prefix_length -= 1
        
        new_mask = subnet_mask_from_prefix_length(new_prefix_length)
        subnet_network = int_to_ip(current_base)
        subnets.append((f"Sieć {subnet_number}", subnet_network, new_mask))
        
        # Przesunięcie bazy o rozmiar podsieci
        current_base += 2 ** (32 - new_prefix_length)
        subnet_number += 1
    
    return subnets

def main():
    base_network = input("Insert base network IP:  ")
    base_mask = input("Insert subnet mask: ")
    sizes = [int(s) for s in input("Insert subnets sizes separated by ',': ").split(',')]
    
    subnets = calculate_subnets(base_network, base_mask, sizes)
    for subnet_number, network, mask in subnets:
        network_address = get_network_address(network, mask)
        broadcast_address = get_broadcast_address(network, mask)
        first_host = int_to_ip(ip_to_int(network_address) + 1)
        last_host = int_to_ip(ip_to_int(broadcast_address) - 1)
        print(f"{subnet_number}: {network}/{mask}\n"
              f"Pierwszy adres użyteczny: {first_host}\n"
              f"Ostatni adres użyteczny: {last_host}\n"
              f"Adres broadcast: {broadcast_address}\n"
              f"{'-' * 70}")  # Linia oddzielająca informacje o różnych podsieciach

if __name__ == '__main__':
    main()

