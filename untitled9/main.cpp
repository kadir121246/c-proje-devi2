/* fopen uyarısını susturmak için */
#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>   // printf, scanf, fopen için
#include <cstdlib>  // exit için
#include <vector>   // Vector (akıllı dizi) için
#include <cmath>    // sqrt için
#include <cstring>  // strcpy vb. için

using namespace std;

struct Ogrenci {
    char no[30];
    char ad[30];
    char soyad[30];
    double quiz;
    double standart_not;
};

// Prototipler
void ekleDegistir(vector<Ogrenci>& ogr);
void siralayarakYazdir(vector<Ogrenci>& ogr);

int main() {
    // 24 kişilik vektör oluşturuyoruz
    vector<Ogrenci> ogrenciler(24);

    printf("Program baslatildi...\n");

    // 1. Adım: Okuma ve Hesaplama
    ekleDegistir(ogrenciler);

    // 2. Adım: Sıralama ve Yazma
    siralayarakYazdir(ogrenciler);

    printf("Islem tamamlandi. Sonuclar 'stdquiz.txt' dosyasina yazildi.\n");

    return 0;
}

void ekleDegistir(vector<Ogrenci>& ogr) {
    FILE *dosya = fopen("quiz.txt", "r");

    if (dosya == NULL) {
        printf("HATA: 'quiz.txt' dosyasi acilamadi!\n");
        printf("Lutfen CLion'da 'Working Directory' ayarini kontrol et.\n");
        exit(1);
    }

    double toplamPuan = 0.0;
    double karelerToplami = 0.0;
    int n = 0;

    // Dosyadan fscanf ile okuma yapıyoruz
    // Vektörün içine (ogr[n]) doğrudan yazıyoruz
    while (n < 24 && fscanf(dosya, "%s %s %s %lf",
                            ogr[n].no,
                            ogr[n].ad,
                            ogr[n].soyad,
                            &ogr[n].quiz) != EOF) {

        toplamPuan += ogr[n].quiz;
        karelerToplami += (ogr[n].quiz * ogr[n].quiz);
        n++;
    }
    fclose(dosya);

    if (n == 0) {
        printf("HATA: Dosyadan veri okunamadi.\n");
        exit(1);
    }

    // Matematiksel Hesaplamalar
    double ortalama = toplamPuan / n;

    double pay = (n * karelerToplami) - (toplamPuan * toplamPuan);
    double payda = n * (n - 1);

    if (pay < 0) pay = 0;

    double standartSapma = sqrt(pay / payda);

    // Z-Puanı Hesaplama
    for (int i = 0; i < n; i++) {
        if (standartSapma != 0) {
            ogr[i].standart_not = (ogr[i].quiz - ortalama) / standartSapma;
        } else {
            ogr[i].standart_not = 0.0;
        }
    }
}

void siralayarakYazdir(vector<Ogrenci>& ogr) {
    // Vektördeki dolu eleman sayısını bulmamız lazım.

    // Basit Bubble Sort (vector üzerinde işlem yapıyoruz)
    // Sadece dolu olanları sıralamak için küçük bir kontrol:
    int doluSayisi = 0;
    for(int i=0; i<24; i++) {
        // Eğer öğrenci numarası boş değilse doludur
        if(strlen(ogr[i].no) > 0) doluSayisi++;
    }

    for (int i = 0; i < doluSayisi - 1; i++) {
        for (int j = 0; j < doluSayisi - i - 1; j++) {
            if (ogr[j].standart_not < ogr[j + 1].standart_not) {
                // C++'ın swap fonksiyonu çok pratiktir, bunu kullanalım
                // İki struct'ı komple yer değiştirir.
                swap(ogr[j], ogr[j + 1]);
            }
        }
    }

    // Dosyaya Yazma (fprintf ile)
    FILE *cikis = fopen("stdquiz.txt", "w");
    if (cikis == NULL) {
        printf("HATA: 'stdquiz.txt' dosyasi olusturulamadi.\n");
        exit(1);
    }

    // Başlık
    fprintf(cikis, "%-15s %-15s %-15s %-10s\n", "No", "Ad", "Soyad", "Z-Puan");
    fprintf(cikis, "-------------------------------------------------------\n");

    for (int i = 0; i < doluSayisi; i++) {
        fprintf(cikis, "%-15s %-15s %-15s %.2lf\n",
                ogr[i].no,
                ogr[i].ad,
                ogr[i].soyad,
                ogr[i].standart_not);
    }

    fclose(cikis);
}